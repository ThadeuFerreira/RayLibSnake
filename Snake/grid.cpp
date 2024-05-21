#include <raylib.h>
#include <deque>
#include <raymath.h>
#include <iostream>

#pragma once
using namespace std;

// Vintage Monitor Green

const Color SNAKE_COLOR = { 43, 51, 24, 255 };
const Color BACKGROUND_COLOR = { 173, 204, 96, 255 };

const int CELL_SIZE = 20;
const int CELL_COUNT_X = 40;
const int CELL_COUNT_Y = 30;

static double lastUpdateTimeStamp = 0;


class Snake;
class Food;
class Grid;

class Food {
private:
	int cellsX;
	int cellsY;
	int cellSize;
public:
	Vector2 position;
	Food(Vector2 position, int cellsX, int cellsY, int cellSize) {
		this->cellsX = cellsX;
		this->cellsY = cellsY;
		this->position = position;
		this->cellSize = cellSize;
	}
	void Draw(Vector2 offset) {
		DrawRectangle(offset.x + position.x * cellSize, offset.y + position.y * cellSize, cellSize - 1, cellSize - 1, SNAKE_COLOR);
	}

	void Respawn(deque<Vector2> *snake) {
		while (true) {
			// Get random position (x, y)
			int x = GetRandomValue(0, cellsX - 1);
			int y = GetRandomValue(0, cellsY - 1);
			position = Vector2{ (float)x, (float)y };
			bool valid = true;
			for (int i = 0; i < snake->size(); i++) {
				if (Vector2Equals(position, (*snake)[i])) {
					valid = false;
					break;
				}
			}
			if (valid) {
				break;
			}
		}
		
	}
};

class Snake {
private:
	int bodyLength;
	Vector2 direction;
	void setDirection(Vector2 newDirection) {
		if (direction.x == 0 && newDirection.x != 0) {
			direction = newDirection;
		}
		else if (direction.y == 0 && newDirection.y != 0) {
			direction = newDirection;
		}
	}

	void reset() {
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
public:
	Vector2 headPosition;
	deque<Vector2> body;
	Color color;
	bool eating;
	float speed;

	Snake() {
		reset();

	}
	void Draw(Vector2 offset) {
		for (int i = 0; i < body.size(); i++) {
			DrawRectangle(offset.x + body[i].x * CELL_SIZE,offset.y + body[i].y * CELL_SIZE, CELL_SIZE - 1, CELL_SIZE - 1, color);
		}
	}
	void Move(Food* food, int cellsX, int cells) {
		if (CheckCollision(cellsX, cells)) {
			reset();
		}
		if (CheckFoodCollision(food)) {
			body.push_front(headPosition);
			this->color = RED;
			this->eating = true;
			bodyLength++;
		}
		
		Vector2 newHeadPosition = Vector2Add(headPosition, direction);
		body.push_front(headPosition);
		headPosition = newHeadPosition;
		if (body.size() > bodyLength) {
			body.pop_back();
		}
	}

	bool CheckFoodCollision(Food* food) {
		if (Vector2Equals(food->position, headPosition)) {
			bodyLength++;
			food->Respawn(&this->body);
			return true;
		}
		return false;
	}

	bool CheckCollision(int cellsX, int cells) {
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

	void Update(Food *food, int cellsX, int cellsY) {
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
		if (time_s2 > 1/speed) {
			Move(food, cellsX, cellsY);
			time_s2 = 0;
		}
	}
};
class Grid {
private:
	Vector2 offset; // top left corner
	int width;
	int height;
	int cellSize;
	Color color;
	int** grid;
	Food* food;
	Snake* snake;

public:
	Grid(Vector2 offset, int width, int height, int cellSize, Color color) {
		this->offset = offset;
		this->width = width;
		this->height = height;
		this->cellSize = cellSize;
		this->color = color;
		grid = new int* [width];
		for (int i = 0; i < width; i++) {
			grid[i] = new int[height];
			for (int j = 0; j < height; j++) {
				grid[i][j] = 0;
			}
		}
		food = new Food(Vector2{ 10, 10 }, width, height, cellSize);
		snake = new Snake();
	}

	void SetFood(Food* food) {
		this->food = food;
	}

	void SetSnake(Snake* snake) {
		this->snake = snake;
	}

	void Update() {
		snake->Update(food, width, height);
	}

	void Draw() {
		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {			
				DrawRectangle(offset.x + i * cellSize, offset.y + j * cellSize, cellSize - 1, cellSize -1, color);
			}
		}
		food->Draw(offset);
		snake->Draw(offset);
	}

	Grid() = default;
};

