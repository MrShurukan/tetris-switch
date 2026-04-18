#include "game.hpp"

static inline Food getRandomFood() {
    return static_cast<Food>(GetRandomValue(0, static_cast<int>(Food::_COUNT) - 1));
}

Game::Game() : currentPiece(getRandomFood()), pieceFallTimer(PIECE_FALL_TIMER_DEFAULT) {
    this->svechka = LoadTexture("romfs:/resources/svechka.png");
    this->tetraminos = LoadTexture("romfs:/resources/tetramino.png");

    this->reset();
}

Game::~Game() {
    UnloadTexture(this->svechka);
    UnloadTexture(this->tetraminos);
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

        int totalCleared = 0;

        for (int y = GRID_HEIGHT - 1; y >= 0; y--) {
            bool full = true;
            for (int x = 0; x < GRID_WIDTH; x++) {
                if (!this->grid[x][y].isFilled) {
                    full = false;
                    break;
                }
            }

            if (!full) continue;

            totalCleared++;

            for (int yy = y; yy > 0; yy--) {
                for (int x = 0; x < GRID_WIDTH; x++) {
                    this->grid[x][yy] = this->grid[x][yy - 1];
                }
            }

            for (int x = 0; x < GRID_WIDTH; x++) {
                this->grid[x][0].isFilled = false;
            }

            // Check same row again after collapse
            y++;
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

        // Destroy the box if it's out of bounds
        if (box->coords.y >= MAIN_FIELD_END_Y + CELL_SIZE * 2) {
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
        float rotationSpeed = GetRandomValue(20, 80) / 10.0f;
        Vector2 speed = (Vector2){(float)GetRandomValue(-100, 100), (float)GetRandomValue(-200, -50)};

        int coordsX = MAIN_FIELD_START_X + x * CELL_SIZE;
        int coordsY = MAIN_FIELD_START_Y + y * CELL_SIZE;

        this->flyingBoxes.push_back(FlyingBox(coordsX, coordsY, cell.textureX, cell.textureY, speed, rotationSpeed));
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
    // After a couple of boxes flying this should get faster
    if (this->flyingBoxes.size() < 10) {
        this->pieceFallTimer.reset(1.0f / GetRandomValue(5, 10));
    }
    else {
        this->pieceFallTimer.reset(1.0f / GetRandomValue(30, 50));
    }
}