#pragma once

#include "game_types.hpp"
#include "tetramino.hpp"
#include "raylib.h"

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
    Cell grid[GRID_WIDTH][GRID_HEIGHT];
    Texture2D svechka;
    // A lookup key for the next food template
    Food nextPieceFood;
    Tetramino currentPiece;
    Timer pieceFallTimer;
    int score;

    // Flying boxes in the end
    std::vector<FlyingBox> flyingBoxes;
    int nextFlyingBoxCoords;

    void drawPressStart() const;
    void drawPlaying() const;
    void drawEnding() const;
    void drawCommon() const;

    void updatePlaying(float deltaTime);
    void updateEnding(float deltaTime);
};