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
        this->score += (totalCleared == 4) ? 10000 : totalCleared * 1000; 

        // Create a new current piece from next one
        Tetramino newTetramino = Tetramino(this->nextPieceFood);
        // Check if new piece doesn't intersect anything
        if (newTetramino.intersectsGrid(this->grid)) {
            // Game Over
            this->reset();
            return;
        }

        this->currentPiece = newTetramino;

        // Generate a new next piece
        this->nextPieceFood = getRandomFood();
    }
}

void Game::draw() const {
    switch (this->state) {
        case GameState::PressStart:
            break;

        case GameState::Playing:
            this->drawPlaying();
            break;
    }
}

Color getBaseColorByFood(Food food) {
    static std::array<Color, static_cast<size_t>(Food::_COUNT)> baseColorByFood = {
        // Kolbasa
        (Color){204, 136, 153, 255},
        // Fish
        (Color){153, 153, 255, 255},
        // Sausages
        (Color){234, 170, 166, 255},
        // Shrimp
        (Color){253, 195, 158, 255},
        // Cheese
        (Color){246, 217, 142, 255},
        // Ryazhenka
        (Color){251, 249, 244, 255},
        // Steak
        (Color){255, 168, 175, 255},
        // DryFood
        (Color){209, 146, 97, 255},
    };

    return baseColorByFood[static_cast<size_t>(food)];
}

static std::string getLabelByFood(Food food) {
    static std::array<std::string, static_cast<size_t>(Food::_COUNT)> labelByFood = {
        // Kolbasa,
        "K",   
        // Fish,
        "F",      
        // Sausages,
        "Sa",   
        // Shrimp,
        "Sh",     
        // Cheese,
        "Ch",     
        // Ryazhenka,
        "R", 
        // Steak,
        "St",      
        // DryFood,
        "DF",    
    };

    return labelByFood[static_cast<size_t>(food)];
}

void drawCell(int x, int y, Food food) {
    Color c = getBaseColorByFood(food);
    DrawRectangle(MAIN_FIELD_START_X + x * CELL_SIZE, MAIN_FIELD_START_Y + y * CELL_SIZE, CELL_SIZE, CELL_SIZE, c);

    // Make it darker
    c.r *= 0.8;
    c.g *= 0.8;
    c.b *= 0.8;
    drawTextCentered(getLabelByFood(food), MAIN_FIELD_START_X + x * CELL_SIZE + CELL_SIZE / 2, MAIN_FIELD_START_Y + y * CELL_SIZE + CELL_SIZE / 2, 24, c);
}

// Left up is svechka's box
// Left down is debug info
// Right top is next block
// Right bottom is score
// Middle is tetris game screen
void Game::drawPlaying() const {
    // TODO: Maybe some fancy background?
    ClearBackground(WHITE);

    // Specator Frame
    DrawRectangle(SPECTATOR_FRAME_START_X, SPECTATOR_FRAME_START_Y, SPECTATOR_FRAME_SIZE, SPECTATOR_FRAME_SIZE, BLACK);
    // Spectator
    drawSquareTexture(this->svechka, (Rectangle){SPECTATOR_START_X, SPECTATOR_START_Y, SPECTATOR_SIZE, SPECTATOR_SIZE});
    drawTextCentered("Svechka the Honorable", SPECTATOR_FRAME_START_X + SPECTATOR_FRAME_SIZE / 2, SPECTATOR_FRAME_END_Y + 20, 24, BLACK);

    // Next piece Frame
    DrawRectangle(NEXT_PIECE_FRAME_START_X, NEXT_PIECE_FRAME_START_Y, NEXT_PIECE_FRAME_SIZE, NEXT_PIECE_FRAME_SIZE, BLACK);
    // Next piece
    DrawRectangle(NEXT_PIECE_START_X, NEXT_PIECE_START_Y, NEXT_PIECE_SIZE, NEXT_PIECE_SIZE, WHITE);
    getTemplateByFood(this->nextPieceFood).drawMiniature(NEXT_PIECE_START_X + 10, NEXT_PIECE_START_Y + 10, NEXT_PIECE_SIZE - 10 * 2);
    drawTextCentered("Next piece", NEXT_PIECE_FRAME_START_X + NEXT_PIECE_FRAME_SIZE / 2, NEXT_PIECE_FRAME_START_Y - 20, 24, BLACK);

    // Score
    std::stringstream score;
    score << "score:\n" << std::setw(6) << std::setfill('0') << this->score;
    drawTextCentered(score.str(), NEXT_PIECE_FRAME_START_X + NEXT_PIECE_FRAME_SIZE / 2, HEIGHT - 100, 48, BLACK);

    // Main field background
    DrawRectangle(MAIN_FIELD_START_X, MAIN_FIELD_START_Y, MAIN_FIELD_WIDTH, MAIN_FIELD_HEIGHT, (Color){0, 0, 0, 255});

    // Grid
    for (int x = 0; x < GRID_WIDTH; x++) {
        for (int y = 0; y < GRID_HEIGHT; y++) {
            if (!grid[x][y].isFilled) continue;

            drawCell(x, y, grid[x][y].food);
        }
    }

    // Active piece
    this->currentPiece.draw();

    // DEBUG
    // std::stringstream stream;
    // stream << "Timer: " << std::fixed << std::setprecision(3) << this->pieceFallTimer.getTimeLeft();
    // DrawText(stream.str().c_str(), 10, HEIGHT - 50, 24, BLACK);
}