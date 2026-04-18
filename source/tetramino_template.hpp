#pragma once

#include "game_types.hpp"
#include "raylib.h"
#include <array>

// Important! Do not make bounding boxes larger than this (or in any other way it will cause the number of items to be larger)
constexpr int MAX_TETRAMINO_BOXES = 5 * 5;

class TetraminoTemplate {
public:
    // For rotating tetraminos (shouldRotate = true)
    TetraminoTemplate(int width, int height, Food type, std::array<bool, MAX_TETRAMINO_BOXES> boxes, GenericVector<int> pivotCoords, int textureX, int textureY);
    // For non-rotation tetraminos (shouldRotate = false)
    TetraminoTemplate(int width, int height, Food type, std::array<bool, MAX_TETRAMINO_BOXES> boxes, int textureX, int textureY);

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
    // Where the graphics are located in the texture atlas (in grid cells, not absolute x and y)
    const int textureX;
    const int textureY;
private:
    // Full constructor
    TetraminoTemplate(int width, int height, Food type, std::array<bool, MAX_TETRAMINO_BOXES> boxes, GenericVector<int> pivotCoords, bool shouldRotate, int textureX, int textureY);

    // Grid of exactly width*height with tetraminoBoxes in it. True if there is a box, false if not.
    const std::array<bool, MAX_TETRAMINO_BOXES> boxes;
};

const TetraminoTemplate& getTemplateByFood(Food food);
Color getBaseColorByFood(Food food);