#include "game.hpp"

TetraminoTemplate::TetraminoTemplate(int width, int height, Food type, std::array<bool, MAX_TETRAMINO_BOXES> boxes)
    : TetraminoTemplate(width, height, type, boxes, GenericVector<int>(0, 0), false) {}

TetraminoTemplate::TetraminoTemplate(int width, int height, Food type, std::array<bool, MAX_TETRAMINO_BOXES> boxes, GenericVector<int> pivotCoords)
    : TetraminoTemplate(width, height, type, boxes, pivotCoords, true) {}

// Full constructor
TetraminoTemplate::TetraminoTemplate(int width, int height, Food type, std::array<bool, MAX_TETRAMINO_BOXES> boxes, GenericVector<int> pivotCoords, bool shouldRotate)
    : width(width), height(height), type(type), pivotCoords(pivotCoords), shouldRotate(shouldRotate), boxes(boxes) 
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
            GenericVector<int>(0, 2)
        ),
        // Reversed L
        TetraminoTemplate(2, 3, Food::Fish, 
            std::array<bool, MAX_TETRAMINO_BOXES> {
                false, true,
                false, true,
                true,  true
            }, 
            GenericVector<int>(1, 1)
        ),
        // L
        TetraminoTemplate(2, 3, Food::Sausages, 
            std::array<bool, MAX_TETRAMINO_BOXES> {
                true, false,
                true, false,
                true, true
            }, 
            GenericVector<int>(0, 1)
        ),
        // Box
        TetraminoTemplate(2, 2, Food::Shrimp, 
            std::array<bool, MAX_TETRAMINO_BOXES> {
                true, true,
                true, true
            }
        ),
        // Pipe _--
        TetraminoTemplate(3, 2, Food::Cheese, 
            std::array<bool, MAX_TETRAMINO_BOXES> {
                false, true,  true,
                true,  true,  false
            },
            GenericVector<int>(1, 1)
        ),
        // T
        TetraminoTemplate(3, 2, Food::Ryazhenka, 
            std::array<bool, MAX_TETRAMINO_BOXES> {
                true,  true, true,
                false, true, false,
            },
            GenericVector<int>(1, 0)
        ),
        // Pipe --_
        TetraminoTemplate(3, 2, Food::Steak, 
            std::array<bool, MAX_TETRAMINO_BOXES> {
                true,  true,  false,
                false, true,  true,
            },
            GenericVector<int>(1, 1)
        ),
        // Single Cell
        TetraminoTemplate(1, 1, Food::DryFood, 
            std::array<bool, MAX_TETRAMINO_BOXES> {
                true,
            }
        ),
    };

    return tetraminos[static_cast<size_t>(food)];
}