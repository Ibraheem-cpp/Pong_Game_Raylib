#include <iostream>
#include "raylib.h"

using namespace std;

const int height = 720;
const int width = 1280;

class Ball;

class Paddle {
protected:
    float Pos_X;
    float Pos_Y;
    float Width;
    float Height;
    float speed = 7;
    int score = 0;
public:
    Paddle() : Pos_X(10), Pos_Y(10), Width(10), Height(20) {};
    Paddle(int Px, int Py, int w, int h) : Pos_X(Px), Pos_Y(Py), Width(w), Height(h) {};

    virtual void draw() = 0;
    virtual void update(int ball_y) = 0;

    virtual bool detect_collision(const Ball& b) const = 0;

    float get_X() const {
        return this->Pos_X;
    }
    float get_Y() const {
        return this->Pos_Y;
    }
    float get_width() const {
        return this->Width;
    }
    int get_score() const {
        return this->score;
    }

    void add_score() {
        this->score++;
    }

    virtual ~Paddle() {};
};


class Ball {
private:
    float x_Coord;
    float y_Coord;
    float radius;
    float speed_x = 8;
    float speed_y = 8;
public:
    Ball() : x_Coord(10), y_Coord(10), radius(10.0) {};
    Ball(int x, int y, float z) : x_Coord(x), y_Coord(y), radius(z) {};

    void draw() const {
        DrawCircle(x_Coord, y_Coord, radius, YELLOW);
    }

    void update(Paddle* player, Paddle* cpu) {
        x_Coord += speed_x;
        y_Coord += speed_y;

        if (y_Coord + radius >= height || y_Coord - radius <= 0) {
            speed_y *= -1;
        }

        if (x_Coord + radius >= width) {
            cpu->add_score();
            x_Coord = float(width) / 2;
            y_Coord = float(height) / 2;
            speed_x *= -1;
        }
        if (x_Coord - radius <= 0) {
            player->add_score();
            x_Coord = float(width) / 2;
            y_Coord = float(height) / 2;
            speed_x *= -1;
        }
    }

    void reverse_direction() {
        speed_x *= -1;
    }

    void set_X(float new_X) {
        this->x_Coord = new_X;
    }

    float get_Y_Coord() const {
        return this->y_Coord;
    }
    float get_X_Coord() const {
        return this->x_Coord;
    }
    float get_rad() const {
        return this->radius;
    }

};

class player_paddle : public Paddle {
public:
    player_paddle() : Paddle() {};
    player_paddle(int Px, int Py, int w, int h) : Paddle(Px, Py, w, h) {};

    void draw() override {
        DrawRectangleRounded(Rectangle{ this->Pos_X, this->Pos_Y, this->Width, this->Height }, 0.8, 0, GREEN);
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

    bool detect_collision(const Ball& b) const override {
        if ((b.get_X_Coord() + b.get_rad() >= this->Pos_X) && (b.get_Y_Coord() >= this->Pos_Y) && (b.get_Y_Coord() <= this->Height + this->Pos_Y)) {
            return true;
        }

        return false;
    }

    ~player_paddle() override {};
};

class AI_paddle : public Paddle {
public:
    AI_paddle() : Paddle() {};
    AI_paddle(int Px, int Py, int w, int h) : Paddle(Px, Py, w, h) { };

    void draw() override {
        DrawRectangleRounded(Rectangle{ this->Pos_X, this->Pos_Y, this->Width, this->Height }, 0.8, 0, GREEN);
    }

    void update(int ball_y) override {
        if (this->Pos_Y + this->Height / 2 > ball_y) {
            this->Pos_Y -= speed;
        }
        if (this->Pos_Y + this->Height / 2 <= ball_y) {
            this->Pos_Y += speed;
        }

        if (Pos_Y <= 0) {
            this->Pos_Y = 0;
        }
        if (Pos_Y + Height >= height) {
            this->Pos_Y = height - this->Height;
        }
    }

    bool detect_collision(const Ball& b) const override {
        if ((b.get_X_Coord() - b.get_rad() <= this->Pos_X + this->Width) && (b.get_Y_Coord() >= this->Pos_Y) && (b.get_Y_Coord() <= this->Height + this->Pos_Y)) {
            return true;
        }

        return false;
    }

    ~AI_paddle() override {};
};


int main()
{
    
    SetTargetFPS(60);
    InitWindow(width, height, "Pong Game");

    Ball ball(width / 2, height / 2, 12.0);
    Paddle* player = new player_paddle(width - 30, height / 2 - 50, 20, 100);
    Paddle* AI = new AI_paddle(10, height / 2 - 50, 20, 100);

    while (!WindowShouldClose()) {

        //      Collision Detection
        if (player->detect_collision(ball)) {
            ball.reverse_direction();
            ball.set_X(player->get_X() - ball.get_rad() - 1);
        }
        if (AI->detect_collision(ball)) {
            ball.reverse_direction();
            ball.set_X(AI->get_X() + AI->get_width() + ball.get_rad() + 1);
        }

        //      Update
        ball.update(player,AI);
        AI->update(ball.get_Y_Coord());
        player->update(ball.get_Y_Coord());

        //      Drawing 
        BeginDrawing();
        ClearBackground(BLUE);
        DrawRectangle(width / 2, 0, width / 2, height, DARKBLUE);
        DrawCircle(width / 2, height / 2, 150, SKYBLUE);
        DrawLine(width / 2 - 1, 0, width / 2, height, WHITE);
        DrawLine(width / 2, 0, width / 2, height, WHITE);
        DrawLine(width / 2 + 1, 0, width / 2, height, WHITE);
        ball.draw();
        AI->draw();
        player->draw();
        DrawText(TextFormat("%i", AI->get_score()),width/4-20,20,80,RED);
        DrawText(TextFormat("%i", player->get_score()), (width / 4) * 3 - 20, 20, 80, RED);
        EndDrawing();
    }


    CloseWindow();

    delete player;
    delete AI;

    return 0;
}
