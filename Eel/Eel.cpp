// Eel.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <raylib.h>

using namespace std;

Color blue = { 0, 142, 148, 255 };
Color brownishYellow = { 150, 120, 0, 255 };
Color orange = { 255, 165, 0, 255 };

int cellSize = 30;
int cellCount = 25;

class Food
{
public: 
	Vector2 position = { 5, 6 };
	Texture2D texture;

	Food()
	{
		Image image = LoadImage("CSE350Graphics/tempFood.png");
		texture = LoadTextureFromImage(image);
		UnloadImage(image);
	}

	~Food()
	{
		UnloadTexture(texture);
	}
	void Draw()
	{
		DrawTexture(texture, position.x * cellSize, position.y * cellSize, WHITE); //texture, x (top left), y (top left), tint
	}
};

int main()
{
	cout << "Eel..." << endl;
	InitWindow(cellSize*cellCount, cellSize*cellCount, "Eel");
	SetTargetFPS(60);

	Food food = Food();

	while (WindowShouldClose() == false)
	{
		BeginDrawing();

		//Drawing
		ClearBackground(blue);
		food.Draw();

		EndDrawing();
	}

	CloseWindow();
	return 0;
}
