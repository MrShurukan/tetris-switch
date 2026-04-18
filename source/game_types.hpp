#pragma once
#include "raylib.h"
#include "shurulib/shurulib.h"
#include <cassert>
#include <map>
#include <sstream>
#include <iomanip>

constexpr int WIDTH = 1280;
constexpr int HEIGHT = 720;

constexpr int CELL_SIZE   = 48;
constexpr int GRID_WIDTH  = 10;
constexpr int GRID_HEIGHT = 15;
constexpr int MAIN_FIELD_WIDTH    = CELL_SIZE * GRID_WIDTH;
constexpr int MAIN_FIELD_HEIGHT   = CELL_SIZE * GRID_HEIGHT;
constexpr int MAIN_FIELD_START_X  = ((WIDTH / 2) - MAIN_FIELD_WIDTH / 2);
constexpr int MAIN_FIELD_END_X    = ((WIDTH / 2) + MAIN_FIELD_WIDTH / 2);
constexpr int MAIN_FIELD_START_Y  = (0      + ((HEIGHT - MAIN_FIELD_HEIGHT) / 2));
constexpr int MAIN_FIELD_END_Y    = (HEIGHT - ((HEIGHT - MAIN_FIELD_HEIGHT) / 2));

constexpr int SPECTATOR_FRAME_SIZE = 200;
constexpr int SPECTATOR_FRAME_START_X = MAIN_FIELD_START_X / 2 - SPECTATOR_FRAME_SIZE / 2;
constexpr int SPECTATOR_FRAME_START_Y = 50;
constexpr int SPECTATOR_FRAME_END_X   = SPECTATOR_FRAME_START_X + SPECTATOR_FRAME_SIZE;
constexpr int SPECTATOR_FRAME_END_Y   = SPECTATOR_FRAME_START_Y + SPECTATOR_FRAME_SIZE;

constexpr int SPECTATOR_PADDING = 20;
constexpr int SPECTATOR_SIZE      = SPECTATOR_FRAME_SIZE - SPECTATOR_PADDING * 2;
constexpr int SPECTATOR_START_X   = SPECTATOR_FRAME_START_X + SPECTATOR_PADDING;
constexpr int SPECTATOR_START_Y   = SPECTATOR_FRAME_START_Y + SPECTATOR_PADDING;

constexpr int NEXT_PIECE_FRAME_SIZE = 200;
constexpr int NEXT_PIECE_FRAME_START_X = (MAIN_FIELD_END_X + (WIDTH - MAIN_FIELD_END_X) / 2) - NEXT_PIECE_FRAME_SIZE / 2;
constexpr int NEXT_PIECE_FRAME_START_Y = SPECTATOR_FRAME_START_Y;
constexpr int NEXT_PIECE_FRAME_END_X   = NEXT_PIECE_FRAME_START_X + NEXT_PIECE_FRAME_SIZE;
constexpr int NEXT_PIECE_FRAME_END_Y   = NEXT_PIECE_FRAME_START_Y + NEXT_PIECE_FRAME_SIZE;

constexpr int NEXT_PIECE_PADDING = 20;
constexpr int NEXT_PIECE_SIZE    = NEXT_PIECE_FRAME_SIZE - NEXT_PIECE_PADDING * 2;
constexpr int NEXT_PIECE_START_X = NEXT_PIECE_FRAME_START_X + NEXT_PIECE_PADDING;
constexpr int NEXT_PIECE_START_Y = NEXT_PIECE_FRAME_START_Y + NEXT_PIECE_PADDING;

constexpr float PIECE_FALL_TIMER_DEFAULT = 0.70f;
constexpr float PIECE_FALL_TIMER_FAST    = 0.15f;

enum class GameState {
    PressStart,
    Playing,
    // Animation of every block "exploding"
    Ending
};

constexpr GameState DEFAULT_STATE = GameState::Playing;

enum class Food {
    // Long piece
    Kolbasa,
    // Reversed L
    Fish,
    // L
    Sausages,
    // Box
    Shrimp,
    // Pipe _--
    Cheese,
    // T
    Ryazhenka,
    // Pipe --_
    Steak,
    // Single Cell
    DryFood,

    // HELPER
    _COUNT
};

struct Cell {
    Cell() {}
    Cell(bool isFilled, float rotation, int textureX, int textureY)
        : isFilled(isFilled), rotation(rotation), textureX(textureX), textureY(textureY) {}
    
    bool isFilled;
    // Rotation of the image
    float rotation;
    // Location in the atlas (absolute coords)
    int textureX;
    int textureY;
};

// Tetramino box
struct Box {
    Box(int x, int y, int textureX, int textureY, bool isPivot) 
        : coords(GenericVector<int>(x, y)), isPivot(isPivot), textureX(textureX), textureY(textureY) {}
    GenericVector<int> coords;
    // Is origin of rotation for the piece
    bool isPivot;
    // Location in the atlas (absolute coords)
    int textureX;
    int textureY;
};

Color getBaseColorByFood(Food food);


const Vector2 gravity {0.0f, 300.0f};

// Flying tetramino box (game end)
struct FlyingBox {
    FlyingBox() {}
    FlyingBox(int x, int y, int textureX, int textureY, Vector2 initialSpeed, float rotationSpeed) 
        : textureX(textureX), textureY(textureY), coords(Vector2{(float)x, (float)y}), speed(initialSpeed), rotation(0.0f), rotationSpeed(rotationSpeed) {}

    // Absolute coords in atlas for the block
    int textureX;
    int textureY;

    Vector2 coords;
    Vector2 speed;

    float rotation;
    float rotationSpeed;

    void update(float deltaTime) {
        this->coords.x += this->speed.x * deltaTime;
        this->coords.y += this->speed.y * deltaTime;

        this->speed.x += gravity.x * deltaTime;
        this->speed.y += gravity.y * deltaTime;

        this->rotation += this->rotationSpeed * deltaTime;
    }

    void draw(Texture2D tetraminos) const {
        drawSquareTextureRotationAtlas(tetraminos, (Rectangle){this->coords.x, this->coords.y, CELL_SIZE, CELL_SIZE}, this->textureX, this->textureY, this->rotation);
        // drawSquareRotation((Rectangle){this->coords.x, this->coords.y, CELL_SIZE, CELL_SIZE}, this->rotation, getBaseColorByFood(this->food));
        // DrawRectangle(this->coords.x, this->coords.y, CELL_SIZE, CELL_SIZE, getBaseColorByFood(this->food));
    }
};