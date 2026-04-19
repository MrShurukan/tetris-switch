#include "tetramino_template.hpp"

TetraminoTemplate::TetraminoTemplate(int width, int height, Food type, std::array<bool, MAX_TETRAMINO_BOXES> boxes, int textureX, int textureY)
    : TetraminoTemplate(width, height, type, boxes, GenericVector<int>(0, 0), false, textureX, textureY) {}

TetraminoTemplate::TetraminoTemplate(int width, int height, Food type, std::array<bool, MAX_TETRAMINO_BOXES> boxes, GenericVector<int> pivotCoords, int textureX, int textureY)
    : TetraminoTemplate(width, height, type, boxes, pivotCoords, true, textureX, textureY) {}

// Full constructor
TetraminoTemplate::TetraminoTemplate(int width, int height, Food type, std::array<bool, MAX_TETRAMINO_BOXES> boxes, GenericVector<int> pivotCoords, bool shouldRotate, int textureX, int textureY)
    : width(width), height(height), type(type), pivotCoords(pivotCoords), shouldRotate(shouldRotate), textureX(textureX), textureY(textureY), boxes(boxes)
{
    assert(width > 0);
    assert(height > 0);
}

bool TetraminoTemplate::hasBoxAt(int x, int y) const {
    assert(x >= 0 && x < this->width);
    assert(y >= 0 && y < this->height);

    return boxes.at(x + y * this->width);
}

void TetraminoTemplate::drawMiniature(int startX, int startY, int size) const {
    // Calculate the larger of the two - width or height. This will define the box sizes
    int max = std::max(this->width, this->height);
    int boxSize = size / max;

    for (int x = 0; x < this->width; x++) {
        for (int y = 0; y < this->height; y++) {
            if (!this->hasBoxAt(x, y)) continue;

            DrawRectangle(startX + x * boxSize, startY + y * boxSize, boxSize, boxSize, getBaseColorByFood(this->type));
        }
    }
}

const TetraminoTemplate& getTemplateByFood(Food food) {
    static const std::array<TetraminoTemplate, static_cast<size_t>(Food::_COUNT)> tetraminos = {
        // Pipe
        TetraminoTemplate(1, 4, Food::Kolbasa, 
            std::array<bool, MAX_TETRAMINO_BOXES> {
                true,
                true,
                true,
                true,
            }, 
            GenericVector<int>(0, 2),
            0, 0
        ),
        // Reversed L
        TetraminoTemplate(2, 3, Food::Fish, 
            std::array<bool, MAX_TETRAMINO_BOXES> {
                false, true,
                false, true,
                true,  true
            }, 
            GenericVector<int>(1, 1),
            1, 0
        ),
        // L
        TetraminoTemplate(2, 3, Food::Sausages, 
            std::array<bool, MAX_TETRAMINO_BOXES> {
                true, false,
                true, false,
                true, true
            },
            GenericVector<int>(0, 1),
            3, 0
        ),
        // Box
        TetraminoTemplate(2, 2, Food::Shrimp, 
            std::array<bool, MAX_TETRAMINO_BOXES> {
                true, true,
                true, true
            },
            4, 0
        ),
        // Pipe _--
        TetraminoTemplate(3, 2, Food::Cheese, 
            std::array<bool, MAX_TETRAMINO_BOXES> {
                false, true,  true,
                true,  true,  false
            },
            GenericVector<int>(1, 1),
            5, 1
        ),
        // T
        TetraminoTemplate(3, 2, Food::Ryazhenka, 
            std::array<bool, MAX_TETRAMINO_BOXES> {
                true,  true, true,
                false, true, false,
            },
            GenericVector<int>(1, 0),
            7, 0
        ),
        // Pipe --_
        TetraminoTemplate(3, 2, Food::Steak, 
            std::array<bool, MAX_TETRAMINO_BOXES> {
                true,  true,  false,
                false, true,  true,
            },
            GenericVector<int>(1, 1),
            10, 0
        ),
        // Single Cell
        TetraminoTemplate(1, 1, Food::DryFood, 
            std::array<bool, MAX_TETRAMINO_BOXES> {
                true,
            },
            12, 0
        ),
    };

    return tetraminos[static_cast<size_t>(food)];
}