#include <raylib.h>
#include <deque>
#include <raymath.h>
#include "food.hpp"
#include "snake.hpp"


void Snake::setDirection(Vector2 newDirection) {
	if (direction.x == 0 && newDirection.x != 0) {
		direction = newDirection;
	}
	else if (direction.y == 0 && newDirection.y != 0) {
		direction = newDirection;
	}
};

void Snake::reset() {
	color = SNAKE_COLOR;
	headPosition = { 2, 0 };
	body.clear();
	body.push_back(Vector2(headPosition));
	body.push_back(Vector2{ 1, 0 });
	body.push_back(Vector2{ 0, 0 });
	direction = { 1, 0 };
	bodyLength = 3;
	speed = 10.0f;
}


Snake::Snake(int cellSize) {
	this->cellSize = cellSize;
    this->colided = false;
	reset();
}
Snake::Snake(int cellSize, deque<Vector2> body, Vector2 direction, int bodyLength, float speed, int cellsX, int cellsY, Color color, Vector2 headPosition, bool eating) {
    this->cellSize = cellSize;
    this->body = body;
    this->direction = direction;
    this->bodyLength = bodyLength;
    this->speed = speed;
    this->cellsX = cellsX;
    this->cellsY = cellsY;
    this->color = color;
    this->headPosition = headPosition;
    this->eating = eating;
}

Snake::~Snake() {
}
void Snake::Draw(Vector2 offset, int cellSize) {
	for (int i = 0; i < body.size(); i++) {
		DrawRectangle(offset.x + body[i].x * cellSize,offset.y + body[i].y * cellSize, cellSize - 1, cellSize - 1, color);
	}
}
SnakeState Snake::Move(Food* food, int cellsX, int cells) {
	if (CheckCollision(cellsX, cells)) {
        colided = true;
        return DEAD;
    }

	SnakeState returnState = MOVING;
	
	if (CheckFoodCollision(food)) {
		body.push_front(headPosition);
		this->color = RED;
		this->eating = true;
		bodyLength++;
        returnState = EATING;
	}

	Vector2 newHeadPosition = Vector2Add(headPosition, direction);
	body.push_front(headPosition);
	headPosition = newHeadPosition;
	if (body.size() > bodyLength) {
		body.pop_back();
	}
    return returnState;
}

bool Snake::CheckFoodCollision(Food* food) {
	if (Vector2Equals(food->position, headPosition)) {
		bodyLength++;
		food->Respawn(&this->body);
		return true;
	}
	return false;
}

bool Snake::CheckCollision(int cellsX, int cells) {
	if (headPosition.x < 0 || headPosition.x >= cellsX || headPosition.y < 0 || headPosition.y >= cells) {
		return true;
	}
	for (int i = 1; i < body.size(); i++) {
		if (headPosition.x == body[i].x && headPosition.y == body[i].y) {
			return true;
		}
	}
	return false;

}

SnakeState Snake::Update(Food* food, int cellsX, int cellsY) {
	// Check for input
	if (IsKeyPressed(KEY_UP) && (direction.x != 0 && direction.y != 1)) {
		setDirection({ 0, -1 });
	}
	else if (IsKeyPressed(KEY_DOWN) && (direction.x != 0 && direction.y != -1)) {
		setDirection({ 0, 1 });
	}
	else if (IsKeyPressed(KEY_LEFT) && (direction.x != 1 && direction.y != 0)) {
		setDirection({ -1, 0 });
	}
	else if (IsKeyPressed(KEY_RIGHT) && (direction.x != -1 && direction.y != 0)) {
		setDirection({ 1, 0 });
	}

	static float time_s;
	static int count = 0;
	time_s += GetFrameTime();
	count += 1;
	if (time_s > 1) {
		color = SNAKE_COLOR;
		TraceLog(LOG_INFO, "Count: %d", count);
		TraceLog(LOG_INFO, "Time: %f", time_s);
		TraceLog(LOG_INFO, "Delta: %f", GetFrameTime());
		time_s = 0;
		count = 0;
		this->eating = false;
	}

	static float time_s2;
	time_s2 += GetFrameTime();
    SnakeState returnState = MOVING;
	if (time_s2 > 1 / speed) {
		returnState = Move(food, cellsX, cellsY);
		time_s2 = 0;
	}
    return returnState;
}
