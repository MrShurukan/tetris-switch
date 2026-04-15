#include "game.hpp"

Tetramino::Tetramino(Food food, int leftX, int topY) : food(food) {
    const TetraminoTemplate& templ = getTemplateByFood(food);
    // Fill the boxes
    for (int x = 0; x < templ.width; x++) {
        for (int y = 0; y < templ.height; y++) {
            if (!templ.hasBoxAt(x, y)) continue;

            boxes.push_back(GenericVector<int>(leftX + x, topY + y));
        }
    }
}

void Tetramino::draw() const {
    for (auto& location : this->boxes) {
        drawCell(location.x, location.y, this->food);
    }
}