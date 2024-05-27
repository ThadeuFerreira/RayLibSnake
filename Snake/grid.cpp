#include <raylib.h>
#include <deque>
#include <raymath.h>
#include <iostream>
#include "snake.hpp"
#include "food.hpp"
#include <string>

#pragma once
using namespace std;

// Vintage Monitor Green


const Color BACKGROUND_COLOR = { 173, 204, 96, 255 };

const int CELL_SIZE = 40;
const int CELL_COUNT_X = 40;
const int CELL_COUNT_Y = 30;

static double lastUpdateTimeStamp = 0;


class Snake;
class Food;
class Grid;


class Grid {
private:
	Vector2 offset; // top left corner
	int width;
	int height;
	int cellSize;
	Color color;
	int** grid;
	int score = 0;
	Food* food;
	Snake* snake;

	//String Message;
	string message;

public:
	Grid(Vector2 offset, int width, int height, int cellSize, Color color) {
		this->offset = offset;
		this->width = width;
		this->height = height;
		this->cellSize = cellSize;
		this->color = color;
		this->message = "Hello World";
		grid = new int* [width];
		for (int i = 0; i < width; i++) {
			grid[i] = new int[height];
			for (int j = 0; j < height; j++) {
				grid[i][j] = 0;
			}
		}
		food = new Food(Vector2{ 10, 10 }, width, height, cellSize);
		snake = new Snake(CELL_SIZE);
	}

	void Reset() {
		delete food;
		delete snake;
		food = new Food(Vector2{ 10, 10 }, width, height, cellSize);
		snake = new Snake(CELL_SIZE);
	
	}

	void SetFood(Food* food) {
		this->food = food;
	}

	void SetSnake(Snake* snake) {
		this->snake = snake;
	}

	void Update() {
		//Draw text in the middle of the screen
		
		SnakeState state = snake->Update(food, width, height);
		switch (state) {
		case SnakeState::DEAD:
			message = "You died!";
			TraceLog(LOG_INFO, message.c_str());
			Reset();
			break;
		case SnakeState::EATING:
			this->score += 1;
			message = "Score: " + to_string(score);
			break;
		default:
			break;
		}
		
	}

	void Draw() {
		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {			
				DrawRectangle(offset.x + i * cellSize, offset.y + j * cellSize, cellSize - 1, cellSize -1, color);
			}
		}
		food->Draw(offset);
		snake->Draw(offset, CELL_SIZE);
		DrawText(message.c_str(), GetScreenWidth() / 2 - MeasureText(message.c_str(), 20) / 2, GetScreenHeight() / 2 - 50, 20, BLACK);
	}

	Grid() = default;
};

