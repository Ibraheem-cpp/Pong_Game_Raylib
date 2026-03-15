#include <iostream>
#include "raylib.h"

using namespace std;

int main()
{
    const int height = 1280;
    const int width = 720;
    InitWindow(height, width, "Pong Game");

    while (!WindowShouldClose()) {

        BeginDrawing();
        DrawLine(height / 2, 0, height / 2, height, WHITE);
        DrawCircle(height / 2, width / 2, 10.0, WHITE);
        DrawRectangle(10, width / 2 - 50, 20, 100, WHITE);
        DrawRectangle(height-30, width / 2 - 50, 20, 100, WHITE);


        EndDrawing();
    }


    CloseWindow();


    return 0;
}
