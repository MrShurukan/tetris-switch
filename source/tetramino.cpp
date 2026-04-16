#include "game.hpp"
#include "tetramino_template.hpp"

Tetramino::Tetramino(Food food) : food(food) {
    const TetraminoTemplate& templ = getTemplateByFood(food);
    // Fill the boxes
    int xOffset = GRID_WIDTH / 2;
    for (int x = 0; x < templ.width; x++) {
        for (int y = 0; y < templ.height; y++) {
            if (!templ.hasBoxAt(x, y)) continue;

            bool isPivot = templ.pivotCoords.x == x && templ.pivotCoords.y == y;
            boxes.push_back(Box(xOffset + x, y, isPivot));
        }
    }
}

void Tetramino::draw() const {
    for (auto& box : this->boxes) {
        drawCell(box.coords.x, box.coords.y, this->food);
    }
}

bool Tetramino::intersectsGrid(const Cell grid[GRID_WIDTH][GRID_HEIGHT]) const {
    for (auto& box : this->boxes) {
        assert(box.coords.x >= 0 && box.coords.x < GRID_WIDTH);
        assert(box.coords.y >= 0 && box.coords.y < GRID_HEIGHT);

        if (grid[box.coords.x][box.coords.y].isFilled) return true;
    }

    return false;
}

void Tetramino::placeInGrid(Cell grid[GRID_WIDTH][GRID_HEIGHT]) const {
    for (auto& box : this->boxes) {
        assert(box.coords.x >= 0 && box.coords.x < GRID_WIDTH);
        assert(box.coords.y >= 0 && box.coords.y < GRID_HEIGHT);

        grid[box.coords.x][box.coords.y].isFilled = true;
        grid[box.coords.x][box.coords.y].food = this->food;
    }
};

bool Tetramino::tryMoveDown(const Cell grid[GRID_WIDTH][GRID_HEIGHT]) {
    auto boxesClone = std::vector<Box>(this->boxes);

    for (auto& box : boxesClone) {
        box.coords.y += 1;
        if (box.coords.y >= GRID_HEIGHT || grid[box.coords.x][box.coords.y].isFilled) return false;
    }

    this->boxes = boxesClone;
    return true;
}

bool Tetramino::tryMoveX(int dx, const Cell grid[GRID_WIDTH][GRID_HEIGHT]) {
    auto boxesClone = std::vector<Box>(this->boxes);

    for (auto& box : boxesClone) {
        box.coords.x += dx;
        if (box.coords.x < 0 || box.coords.x >= GRID_WIDTH || grid[box.coords.x][box.coords.y].isFilled) return false;
    }

    this->boxes = boxesClone;
    return true;
}

bool Tetramino::tryRotateClockwise(const Cell grid[GRID_WIDTH][GRID_HEIGHT]) {
    if (!getTemplateByFood(this->food).shouldRotate) return false;

    auto boxesClone = std::vector<Box>(this->boxes);

    // Apply rotation
    // For this we need to calculate dx,dy from pivot, rearrange, and put back
    GenericVector<int> pivotLocation;
    bool pivotFound = false;
    
    for (auto& box : this->boxes) {
        if (box.isPivot) {
            pivotLocation = box.coords;
            pivotFound = true;
            break;
        } 
    }

    assert(pivotFound);

    for (auto& box : boxesClone) {
        // Calculating dx,dy from pivot
        box.coords.x -= pivotLocation.x;
        box.coords.y -= pivotLocation.y;

        // Applying the rotation while in dx,dy
        int oldX = box.coords.x;
        box.coords.x = -box.coords.y;
        box.coords.y = oldX;

        // Restoring original position
        box.coords.x += pivotLocation.x;
        box.coords.y += pivotLocation.y;

        if (box.coords.x < 0 || box.coords.x >= GRID_WIDTH || 
            box.coords.y < 0 || box.coords.y >= GRID_HEIGHT ||
            grid[box.coords.x][box.coords.y].isFilled) return false;
    }

    this->boxes = boxesClone;
    return true;
}