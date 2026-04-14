#include "raylib.h"
#include "shurulib/shurulib.h"
#include "game.hpp"

int main(void) {
    InitWindow(WIDTH, HEIGHT, "tetris");
    SetTargetFPS(60);

    Game game;

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        game.processInput();
        game.update(dt);
        
        BeginDrawing();
        game.draw();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}