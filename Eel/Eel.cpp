// Eel.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <raylib.h>
#include <deque>

using namespace std;

Color blue = { 0, 142, 148, 255 };
Color brownishYellow = { 150, 120, 0, 255 };
Color orange = { 255, 165, 0, 255 };

int cellSize = 30;
int cellCount = 25;

class Food
{
public: 
	Vector2 position;
	Texture2D texture;

	Food()
	{
		Image image = LoadImage("CSE350Graphics/tempFood.png");
		texture = LoadTextureFromImage(image);
		UnloadImage(image);
		position = GenerateRandomPos();
	}

	~Food()
	{
		UnloadTexture(texture);
	}
	void Draw()
	{
		DrawTexture(texture, position.x * cellSize, position.y * cellSize, WHITE); //texture, x (top left), y (top left), tint
	}
	Vector2 GenerateRandomPos()
	{
		float x = GetRandomValue(0, cellCount - 1);
		float y = GetRandomValue(0, cellCount - 1);
		return Vector2{ x, y }; 
	}
};

class Snake
{
public:
	deque<Vector2> body = { Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9} }; //a queue that contains cords for snakes body

	void Draw()
	{
		for (unsigned int i = 0; i < body.size(); i++) //body.size returns unsigned int length of the queue
		{
			int x = body[i].x;
			int y = body[i].y;
			DrawRectangle(x * cellSize, y * cellSize, cellSize, cellSize, orange); //x (top left), y (top left), width, height, color
		}
	}
};

int main()
{
	cout << "Eel..." << endl;
	InitWindow(cellSize*cellCount, cellSize*cellCount, "Eel");
	SetTargetFPS(60);

	Food food = Food();
	Snake snake = Snake();

	while (WindowShouldClose() == false)
	{
		BeginDrawing();

		//Drawing
		ClearBackground(blue);
		food.Draw();
		snake.Draw();

		EndDrawing();
	}

	CloseWindow();
	return 0;
}
