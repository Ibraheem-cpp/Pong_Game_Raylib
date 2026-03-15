#include <iostream>
#include "raylib.h"

using namespace std;

const int height = 720;
const int width = 1280;

class Ball {
private:
    int x_Coord;
    int y_Coord;
    float radius;
    int speed_x = 8;
    int speed_y = 8;
public:
    Ball() : x_Coord(10), y_Coord(10), radius(10.0) {};
    Ball(int x, int y, float z) : x_Coord(x), y_Coord(y), radius(z) {};

    void draw() const {
        DrawCircle(x_Coord, y_Coord, radius, GREEN);
    }

    void update() {
        x_Coord += speed_x;
        y_Coord += speed_y;

        if (y_Coord + radius >= height || y_Coord - radius <= 0) {
            speed_y *= -1;
        }
        if (x_Coord + radius >= width || x_Coord - radius <= 0) {
            speed_x *= -1;
        }
    }

    int get_Y_Coord() const {
        return this->y_Coord;
    }
};

class Paddle {
protected:
    float Pos_X;
    float Pos_Y;
    float Width;
    float Height;
    int speed = 7;
public:
    Paddle() : Pos_X(10), Pos_Y(10), Width(10), Height(20) {};
    Paddle(int Px, int Py, int w, int h) : Pos_X(Px), Pos_Y(Py), Width(w), Height(h) {};

    virtual void draw() = 0;
    virtual void update(int ball_y) = 0;
};

class player_paddle : public Paddle {
public:
    player_paddle() : Paddle() {};
    player_paddle(int Px, int Py, int w, int h) : Paddle(Px, Py, w, h) {};

    void draw() override {
        DrawRectangle(this->Pos_X, this->Pos_Y, this->Width, this->Height, GREEN);
    }

    void update(int ball_y) override {
        if (IsKeyDown(KEY_W)) {
            this->Pos_Y -=speed;
        }
        if (IsKeyDown(KEY_S)) {
            this->Pos_Y += speed;
        }

        if (Pos_Y <= 0) {
            this->Pos_Y = 0;
        }
        if (Pos_Y + Height >= height) {
            this->Pos_Y = height - this->Height;
        }
    }

};

class AI_paddle : public Paddle {
public:
    AI_paddle() : Paddle() {};
    AI_paddle(int Px, int Py, int w, int h) : Paddle(Px, Py, w, h) {};

    void draw() override {
        DrawRectangle(this->Pos_X, this->Pos_Y, this->Width, this->Height, GREEN);
    }

    void update(int ball_y) override {
        if (this->Pos_Y > ball_y) {
            this->Pos_Y -= speed;
        }
        if (this->Pos_Y <= ball_y) {
            this->Pos_Y += speed;
        }

        if (Pos_Y <= 0) {
            this->Pos_Y = 0;
        }
        if (Pos_Y + Height >= height) {
            this->Pos_Y = height - this->Height;
        }
    }
};

int main()
{
    
    SetTargetFPS(60);
    InitWindow(width, height, "Pong Game");

    Ball ball(width / 2, height / 2, 15.0);
    Paddle* player = new player_paddle(width - 30, height / 2 - 50, 20, 100);
    Paddle* AI = new AI_paddle(10, height / 2 - 50, 20, 100);

    while (!WindowShouldClose()) {

        //      Update
        ball.update();
        AI->update(ball.get_Y_Coord());
        player->update(ball.get_Y_Coord());

        //      Drawing
        ClearBackground(DARKBLUE);
        BeginDrawing();
        DrawLine(width / 2, 0, width / 2, height, WHITE);
        ball.draw();
        AI->draw();
        player->draw();

        EndDrawing();
    }


    CloseWindow();

    delete player;
    delete AI;

    return 0;
}
