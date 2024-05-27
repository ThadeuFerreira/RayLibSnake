#include <raylib.h>
#include <deque>


#pragma once
using namespace std;

const Color FOOD_COLOR = { 255, 0, 0, 255 };

class Food {
private:
	int cellsX;
	int cellsY;
	int cellSize;
public:
	Vector2 position;
	Food(Vector2 position, int cellsX, int cellsY, int cellSize);
	~Food();
	void Draw(Vector2 offset);
	void Respawn(deque<Vector2>* snake);

};