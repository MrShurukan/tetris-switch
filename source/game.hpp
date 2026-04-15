#pragma once
#include "raylib.h"
#include "shurulib/shurulib.h"
#include <cassert>
#include <map>
#include <sstream>
#include <iomanip>

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

#define NEXT_PIECE_FRAME_SIZE 200
#define NEXT_PIECE_FRAME_START_X (MAIN_FIELD_END_X + (WIDTH - MAIN_FIELD_END_X) / 2) - NEXT_PIECE_FRAME_SIZE / 2
#define NEXT_PIECE_FRAME_START_Y SPECTATOR_FRAME_START_Y
#define NEXT_PIECE_FRAME_END_X   NEXT_PIECE_FRAME_START_X + NEXT_PIECE_FRAME_SIZE
#define NEXT_PIECE_FRAME_END_Y   NEXT_PIECE_FRAME_START_Y + NEXT_PIECE_FRAME_SIZE

#define NEXT_PIECE_PADDING 20
#define NEXT_PIECE_SIZE    NEXT_PIECE_FRAME_SIZE - NEXT_PIECE_PADDING * 2
#define NEXT_PIECE_START_X NEXT_PIECE_FRAME_START_X + NEXT_PIECE_PADDING
#define NEXT_PIECE_START_Y NEXT_PIECE_FRAME_START_Y + NEXT_PIECE_PADDING

#define DEFAULT_STATE GameState::Playing

#define PIECE_FALL_TIMER_DEFAULT 0.7f
#define PIECE_FALL_TIMER_FAST 0.15f

enum class GameState {
    PressStart,
    Playing,
};

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
    bool isFilled;
    Food food;
};

// Important! Do not make bounding boxes larger than this (or in any other way it will cause the number of items to be larger)
#define MAX_TETRAMINO_BOXES 5 * 5

class TetraminoTemplate {
public:
    // For rotating tetraminos (shouldRotate = true)
    TetraminoTemplate(int width, int height, Food type, std::array<bool, MAX_TETRAMINO_BOXES> boxes, GenericVector<int> pivotCoords);
    // For non-rotation tetraminos (shouldRotate = false)
    TetraminoTemplate(int width, int height, Food type, std::array<bool, MAX_TETRAMINO_BOXES> boxes);

    // Access to the boxes in a convenient way
    bool hasBoxAt(int x, int y) const;

    // Draw a miniature tetramino at the specified location. It would be drawn in a bounded square.
    void drawMiniature(int startX, int startY, int size) const;

    // Bounding box width and height in cells
    const int width;
    const int height;
    // Which type this tetramino is (defines the texture)
    const Food type;
    // A marker for which piece is the pivot (origin) around which piece will be rotated
    const GenericVector<int> pivotCoords;
    // If rotation is allowed
    const bool shouldRotate;
private:
    // Full constructor
    TetraminoTemplate(int width, int height, Food type, std::array<bool, MAX_TETRAMINO_BOXES> boxes, GenericVector<int> pivotCoords, bool shouldRotate);

    // Grid of exactly width*height with tetraminoBoxes in it. True if there is a box, false if not.
    const std::array<bool, MAX_TETRAMINO_BOXES> boxes;
};

const TetraminoTemplate& getTemplateByFood(Food food);
Color getBaseColorByFood(Food food);
void drawCell(int x, int y, Food food);

// Tetramino box
struct Box {
    Box(int x, int y, bool isPivot) : coords(GenericVector<int>(x, y)), isPivot(isPivot) {}
    GenericVector<int> coords;
    // Is origin of rotation for the piece
    bool isPivot;
};

class Tetramino {
public:
    // Create a Tetramino with a specified food type. Appears at coords GRID_WIDTH / 2, 0
    Tetramino(Food food);
    // Draw the tetramino on the base field
    void draw() const;
    // Check if tetramino intersects anything in the grid
    bool intersectsGrid(const Cell grid[GRID_WIDTH][GRID_HEIGHT]) const;

    // Transfer current boxes to grid
    void placeInGrid(Cell grid[GRID_WIDTH][GRID_HEIGHT]) const;

    // Try move down; if intersects grid then movement is cancelled and false is returned
    bool tryMoveDown(const Cell grid[GRID_WIDTH][GRID_HEIGHT]);

    // Try move horizontally; if intersects grid then movement is cancelled and false is returned
    bool tryMoveX(int dx, const Cell grid[GRID_WIDTH][GRID_HEIGHT]);

    // Try rotating clockwise; if intersects grid then rotatio nis cancelled and false is returned
    bool tryRotateClockwise(const Cell grid[GRID_WIDTH][GRID_HEIGHT]);
private:
    // A lookup key for the template
    Food food;
    // A list of actual boxes and their coordinates
    std::vector<Box> boxes;
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
    Cell grid[GRID_WIDTH][GRID_HEIGHT];
    Texture2D svechka;
    // A lookup key for the next food
    Food nextPieceFood;
    Tetramino currentPiece;
    Timer pieceFallTimer;
    int score;

    void drawPressStart() const;
    void drawPlaying() const;
};