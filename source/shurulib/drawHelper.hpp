#pragma once
#include "raylib.h"
#include <string>
void drawSquareTexture(Texture texture, Rectangle location);
void drawSquareTextureRotation(Texture texture, Rectangle location, float rotation);
void drawTextCentered(std::string string, int centerX, int centerY, int fontSize, Color color);