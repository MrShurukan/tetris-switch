#include "game.hpp"
#include "tetramino_template.hpp"

void Game::draw() const {
    switch (this->state) {
        case GameState::PressStart:
            break;

        case GameState::Playing:
            this->drawPlaying();
            break;

        case GameState::Ending:
            this->drawEnding();
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
void Game::drawCommon() const {
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

    // Grid guideline
    for (int x = 0; x < GRID_WIDTH; x++) {
        // Vertical grid guideline
        int xGuideline = MAIN_FIELD_START_X + x * CELL_SIZE;
        DrawLine(xGuideline, MAIN_FIELD_START_Y, xGuideline, MAIN_FIELD_END_Y, GRAY);
    }
    for (int y = 0; y < GRID_HEIGHT; y++) {
        // Horizontal grid guideline
        int yGuideline = MAIN_FIELD_START_Y + y * CELL_SIZE;
        DrawLine(MAIN_FIELD_START_X, yGuideline, MAIN_FIELD_END_X, yGuideline, GRAY);
    }

    // Grid fill
    for (int x = 0; x < GRID_WIDTH; x++) {
        for (int y = 0; y < GRID_HEIGHT; y++) {
            if (!grid[x][y].isFilled) continue;

            drawCell(x, y, grid[x][y].food);
        }
    }
}

void Game::drawPlaying() const {
    this->drawCommon();

    // Active piece
    this->currentPiece.draw();

    // DEBUG
    // std::stringstream stream;
    // stream << "Timer: " << std::fixed << std::setprecision(3) << this->pieceFallTimer.getTimeLeft();
    // DrawText(stream.str().c_str(), 10, HEIGHT - 50, 24, BLACK);
}

void Game::drawEnding() const {
    this->drawCommon();
}