#include "game.hpp"

static inline Food getRandomFood() {
    return static_cast<Food>(GetRandomValue(0, static_cast<int>(Food::_COUNT) - 1));
}

Game::Game() : currentPiece(getRandomFood()), pieceFallTimer(PIECE_FALL_TIMER_DEFAULT) {
    this->svechka = LoadTexture("romfs:/resources/svechka.png");

    this->reset();
}

Game::~Game() {
    UnloadTexture(this->svechka);
}

void Game::reset() {
    this->state = DEFAULT_STATE;
    for (int x = 0; x < GRID_WIDTH; x++) {
        for (int y = 0; y < GRID_HEIGHT; y++) {
            this->grid[x][y].isFilled = false;
        }
    }

    this->score = 0;
    this->flyingBoxes.clear(); this->nextFlyingBoxCoords = 0;
    this->nextPieceFood = getRandomFood();
    this->currentPiece = Tetramino(getRandomFood());
    this->pieceFallTimer.reset(PIECE_FALL_TIMER_DEFAULT);
}

void Game::processInput() {
    if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) {
        this->currentPiece.tryRotateClockwise(this->grid);
    }

    if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT)) {
        this->currentPiece.tryMoveX(-1, this->grid);
    }
    else if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT)) {
        this->currentPiece.tryMoveX(1, this->grid);
    }
    
    if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN)) {
        this->pieceFallTimer.triggerAndReset(PIECE_FALL_TIMER_FAST);
    }
    else if (IsGamepadButtonReleased(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN)) {
        this->pieceFallTimer.triggerAndReset(PIECE_FALL_TIMER_DEFAULT);
    }
}

void Game::update(float deltaTime) {
    switch (this->state) {
        case GameState::PressStart:
            break;
        case GameState::Playing:
            this->updatePlaying(deltaTime);
            break;
        case GameState::Ending:
            this->updateEnding(deltaTime);
            break;
    }
}

void Game::updatePlaying(float deltaTime) {
    if (!this->pieceFallTimer.process(deltaTime)) return;

    // Check if piece landed, otherwise just move it down
    if (!this->currentPiece.tryMoveDown(this->grid)) {
        // Place it permanentely in the grid
        this->currentPiece.placeInGrid(this->grid);

        // Check if line(-s) were cleared
        std::array<bool, GRID_HEIGHT> lineCleared;

        int totalCleared = 0;

        for (int y = 0; y < GRID_HEIGHT; y++) {
            lineCleared[y] = true;
            for (int x = 0; x < GRID_WIDTH; x++) {
                if (this->grid[x][y].isFilled) continue;

                lineCleared[y] = false;
                break;
            }
        }
        
        // Clear lines
        for (int y = GRID_HEIGHT - 1; y >= 0; y--) {
            if (!lineCleared[y]) continue;
            totalCleared++;

            for (int x = 0; x < GRID_WIDTH; x++) {
                for (int yPrime = y-1; yPrime >= 0; yPrime--) {
                    this->grid[x][yPrime+1] = this->grid[x][yPrime];
                }
            }
        }

        // Calculate new score
        this->score += (totalCleared == 4) ? 5000 : totalCleared * 500; 

        // Create a new current piece from next one
        Tetramino newTetramino = Tetramino(this->nextPieceFood);
        // Check if new piece doesn't intersect anything
        if (newTetramino.intersectsGrid(this->grid)) {
            // Game Over
            this->state = GameState::Ending;
            return;
        }

        this->currentPiece = newTetramino;

        // Generate a new next piece
        this->nextPieceFood = getRandomFood();
    }
}

void Game::updateEnding(float deltaTime) {
    for (auto box = this->flyingBoxes.begin(); box != this->flyingBoxes.end();) {
        box->update(deltaTime);
        box->draw();

        // Destroy the box if it's out of bounds
        if (box->coords.y >= GRID_HEIGHT + CELL_SIZE * 2) {
            this->flyingBoxes.erase(box);
        }
        else {
            box++;
        }
    }

    // Repurpose the timer for falling pieces for starting explosions
    if (!this->pieceFallTimer.process(deltaTime)) return;

    for (int i = this->nextFlyingBoxCoords; i < GRID_WIDTH * GRID_HEIGHT; i++) {
        int x = i % GRID_WIDTH;
        int y = i / GRID_WIDTH;

        Cell& cell = this->grid[x][y];
        if (!cell.isFilled) continue;

        // Random speed and rotation
        float rotationSpeed = GetRandomValue(20, 80) / 100.0f;
        Vector2 speed = (Vector2){GetRandomValue(-5, 5), GetRandomValue(10, 50)};

        this->flyingBoxes.push_back(FlyingBox(cell.food, x, y, speed, rotationSpeed));
        cell.isFilled = false;

        this->nextFlyingBoxCoords = i + 1;
        break;
    }

    // If none boxes were added - we are out of cells to explode.
    if (this->flyingBoxes.size() == 0) {
        // Game Over
        this->reset();
        return;
    }

    // Randomize next time this happens
    this->pieceFallTimer.reset(1.0f / GetRandomValue(3, 5));
}