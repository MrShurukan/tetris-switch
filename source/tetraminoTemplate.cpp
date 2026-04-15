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

const std::array<TetraminoTemplate, static_cast<size_t>(Food::_COUNT)> tetraminos = {
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
        GenericVector<int>(1, 1)
    ),
    // Box
    TetraminoTemplate(2, 2, Food::Shrimp, 
        std::array<bool, MAX_TETRAMINO_BOXES> {
            true, true,
            true, true
        }
    ),
    // Pipe _--
    TetraminoTemplate(4, 2, Food::Cheese, 
        std::array<bool, MAX_TETRAMINO_BOXES> {
            false, false, true,  true,
            true,  true,  false, false
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
    TetraminoTemplate(4, 2, Food::Steak, 
        std::array<bool, MAX_TETRAMINO_BOXES> {
            true,  true,  false, false,
            false, false, true,  true
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