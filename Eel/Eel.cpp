// Eel.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <raylib.h>
#include <deque>
#include <raymath.h>

using namespace std;

Color blue = { 0, 142, 148, 255 };
Color brownishYellow = { 150, 120, 0, 255 };
Color orange = { 255, 165, 0, 255 };

int cellSize = 30;
int cellCount = 25;

double startTime = 0;

bool timeChecker(double interval) //you will pass in the time and see if it has elapsed
{
	double currentTime = GetTime();
	if (currentTime - startTime >= interval)
	{
		startTime = currentTime;
		return true; //yes, interval has elapsed
	}
		return false; //no, interval has not elapsed
}

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

class Eel
{
public:
	deque<Vector2> body = { Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9} }; //a queue that contains cords for eels body
	Vector2 direction = { 1, 0 };

	void Draw()
	{
		for (unsigned int i = 0; i < body.size(); i++) //body.size returns unsigned int length of the queue
		{
			int x = body[i].x;
			int y = body[i].y;
			DrawRectangle(x * cellSize, y * cellSize, cellSize, cellSize, orange); //x (top left), y (top left), width, height, color
		}
	}
	void Update()
	{
		body.pop_back();
		body.push_front(Vector2Add(body[0], direction)); //this will move the eel's head in the direction specified through vector addition
	}
};

int main()
{
	cout << "Eel..." << endl;
	InitWindow(cellSize*cellCount, cellSize*cellCount, "Eel");
	SetTargetFPS(60);

	Food food = Food();
	Eel eel = Eel();

	while (WindowShouldClose() == false)
	{
		BeginDrawing();

		if (timeChecker(0.2)) //delays game loop
		{
			eel.Update();
		}

		if (IsKeyPressed(KEY_W)) //up
		{
			eel.direction = { 0, -1 };
		}
		if (IsKeyPressed(KEY_A)) //left
		{
			eel.direction = { -1, 0 };
		}
		if (IsKeyPressed(KEY_S)) //down
		{
			eel.direction = { 0, 1 };
		}
		if (IsKeyPressed(KEY_D)) //right
		{
			eel.direction = { 1, 0 };
		}
		//Drawing
		ClearBackground(blue);
		food.Draw();
		eel.Draw();

		EndDrawing();
	}

	CloseWindow();
	return 0;
}
