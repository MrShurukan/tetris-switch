#pragma once

#include "game_types.hpp"
#include <vector>

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