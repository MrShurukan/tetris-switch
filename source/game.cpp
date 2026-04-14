#include "game.hpp"

Game::Game() {
    this->svechka = LoadTexture("romfs:/resources/svechka.png");

    this->reset();
}

Game::~Game() {
    UnloadTexture(this->svechka);
}

void Game::reset() {
    this->state = DEFAULT_STATE;
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


// Left up is svechka's box
// Left down is debug info
// Right top is next block
// Right bottom is score
// Middle is tetris game screen
void Game::drawPlaying() const {
    // TODO: Maybe some fancy background?
    ClearBackground(WHITE);

    // Frame
    DrawRectangle(SPECTATOR_FRAME_START_X, SPECTATOR_FRAME_START_Y, SPECTATOR_FRAME_SIZE, SPECTATOR_FRAME_SIZE, BLACK);
    // Spectator
    drawSquareTexture(this->svechka, (Rectangle){SPECTATOR_START_X, SPECTATOR_START_Y, SPECTATOR_SIZE, SPECTATOR_SIZE});

    drawTextCentered("Svechka the Honorable", SPECTATOR_FRAME_START_X + SPECTATOR_FRAME_SIZE / 2, SPECTATOR_FRAME_END_Y + 20, 24, BLACK);

    // Main field
    DrawRectangle(MAIN_FIELD_START_X, MAIN_FIELD_START_Y, MAIN_FIELD_WIDTH, MAIN_FIELD_HEIGHT, (Color){0, 0, 0, 255});
}