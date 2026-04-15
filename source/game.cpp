#include "game.hpp"

static inline Food getRandomFood() {
    return static_cast<Food>(GetRandomValue(0, static_cast<int>(Food::_COUNT) - 1));
}

Game::Game() : currentPiece(getRandomFood(), 0, 0) {
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

    this->nextPieceFood = getRandomFood();
    this->currentPiece = Tetramino(getRandomFood(), GRID_WIDTH / 2, 0);
}

void Game::processInput() {

}

void Game::update(float deltaTime) {

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
    drawTextCentered("score:\n000000", NEXT_PIECE_FRAME_START_X + NEXT_PIECE_FRAME_SIZE / 2, HEIGHT - 100, 48, BLACK);

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
}