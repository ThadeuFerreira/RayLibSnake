#include "raylib.h"
#include "food.hpp"

#pragma once

using namespace std;

//Enum for the states of the snake
// moving, eating, dead
enum SnakeState {
    MOVING,
    EATING,
    DEAD
};


const Color SNAKE_COLOR = { 43, 51, 24, 255 };

class Snake {
private:
    Color color;
	Vector2 headPosition;
	deque<Vector2> body;
	Vector2 direction;
	int bodyLength;
    bool eating;
	float speed;
	int cellsX;
	int cellsY;
	int cellSize;
	void setDirection(Vector2 newDirection);
    void reset();
    
public:
    bool colided;
    Snake(int cellSize);
    Snake(int cellSize, deque<Vector2> body, Vector2 direction, int bodyLength, float speed, int cellsX, int cellsY, Color color, Vector2 headPosition, bool eating);
    ~Snake();
    void Draw(Vector2 offset, int  cellSize);
    SnakeState Update(Food* food, int cellsX, int cellsY);
    void Reset();
    void SetDirection(Vector2 newDirection);
    void Grow();
    bool CheckCollision(int cellsX, int cells);
    bool CheckFoodCollision(Food* food);
    SnakeState Move(Food* food, int cellsX, int cells);
    void SetSpeed(float speed);
    Vector2 GetHeadPosition();
    Vector2 GetTailPosition();
    int GetLength();
};