#include <raylib.h>
#include "grid.cpp"

using namespace std;

int main()
{
	// Initialization
	//--------------------------------------------------------------------------------------
	/*const int screenWidth = 800;
	const int screenHeight = 600;*/
	
	InitWindow(CELL_COUNT_X * CELL_SIZE, CELL_COUNT_Y * CELL_SIZE, "Snake Game");
	SetTargetFPS(60);

	Vector2 gridOffset = { 100, 0 };
	Grid grid = Grid(gridOffset, CELL_COUNT_X/2, CELL_COUNT_Y, CELL_SIZE, BACKGROUND_COLOR);
	SetTraceLogLevel(LOG_ALL);
	while (WindowShouldClose() == false)
	{ 
		// Update
		BeginDrawing();
		ClearBackground(WHITE);
		grid.Update();
		grid.Draw();
		EndDrawing();
	}

	CloseWindow();
	return 0;

}