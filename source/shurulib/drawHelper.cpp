#include "drawHelper.hpp"

void drawSquareTexture(Texture texture, Rectangle location) {
    DrawTexturePro(texture, (Rectangle){0,0,texture.width,texture.height},
                            location, (Vector2){0,0}, 0, WHITE);
}

void drawSquareTextureRotationAtlas(Texture texture, Rectangle location, int textureX, int textureY, float rotation) {
    location.x += location.width / 2;
    location.y += location.height / 2;
    DrawTexturePro(texture, (Rectangle){textureX,textureY,location.width,location.height},
                            location, (Vector2){location.width / 2, location.height / 2}, rotation, WHITE);
}

void drawSquareTextureRotation(Texture texture, Rectangle location, float rotation) {
    drawSquareTextureRotationAtlas(texture, location, 0, 0, rotation);
}

void drawSquareRotation(Rectangle location, float rotation, Color color) {
    location.x += location.width / 2;
    location.y += location.height / 2;
    DrawRectanglePro(location, (Vector2){location.width / 2, location.height / 2}, rotation, color);
}

void drawTextCentered(std::string text, int centerX, int centerY, int fontSize, Color color) {
    int textWidth = MeasureText(text.c_str(), fontSize);
    DrawText(text.c_str(), centerX - textWidth / 2, centerY - fontSize / 2, fontSize, color);
}