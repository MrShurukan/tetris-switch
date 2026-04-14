#pragma once
#include "raylib.h"
#include "shurulib/shurulib.h"

#define WIDTH 1280
#define HEIGHT 720

#define CELL_SIZE   48
#define GRID_WIDTH  10
#define GRID_HEIGHT 15
#define MAIN_FIELD_WIDTH    CELL_SIZE * GRID_WIDTH
#define MAIN_FIELD_HEIGHT   CELL_SIZE * GRID_HEIGHT
#define MAIN_FIELD_START_X  ((WIDTH / 2) - MAIN_FIELD_WIDTH / 2)
#define MAIN_FIELD_END_X    ((WIDTH / 2) + MAIN_FIELD_WIDTH / 2)
#define MAIN_FIELD_START_Y  (0      + ((HEIGHT - MAIN_FIELD_HEIGHT) / 2))
#define MAIN_FIELD_END_Y    (HEIGHT - ((HEIGHT - MAIN_FIELD_HEIGHT) / 2))

#define SPECTATOR_FRAME_SIZE 200
#define SPECTATOR_FRAME_START_X MAIN_FIELD_START_X / 2 - SPECTATOR_FRAME_SIZE / 2
#define SPECTATOR_FRAME_START_Y 50
#define SPECTATOR_FRAME_END_X   SPECTATOR_FRAME_START_X + SPECTATOR_FRAME_SIZE
#define SPECTATOR_FRAME_END_Y   SPECTATOR_FRAME_START_Y + SPECTATOR_FRAME_SIZE

#define SPECTATOR_PADDING 20
#define SPECTATOR_SIZE      SPECTATOR_FRAME_SIZE - SPECTATOR_PADDING * 2
#define SPECTATOR_START_X   SPECTATOR_FRAME_START_X + SPECTATOR_PADDING
#define SPECTATOR_START_Y   SPECTATOR_FRAME_START_Y + SPECTATOR_PADDING

#define DEFAULT_STATE GameState::Playing

enum class GameState {
    PressStart,
    Playing,
};

class Game {
public:
    Game();
    ~Game();

    void processInput();
    void update(float deltaTime);
    void draw() const;
    void reset();

private:
    GameState state;
    Texture2D svechka;

    void drawPressStart() const;
    void drawPlaying() const;
};