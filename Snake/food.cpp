#include "raylib.h"
#include <deque>
#include <raymath.h>
#include "food.hpp"



Food::Food(Vector2 position, int cellsX, int cellsY, int cellSize) {
		this->cellsX = cellsX;
		this->cellsY = cellsY;
		this->position = position;
		this->cellSize = cellSize;
	}
void Food::Draw(Vector2 offset) {
    DrawRectangle(offset.x + position.x * cellSize, offset.y + position.y * cellSize, cellSize - 1, cellSize - 1, FOOD_COLOR);
}

Food::~Food() {
}

void Food::Respawn(deque<Vector2> *snake) {
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