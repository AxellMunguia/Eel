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

bool FoodInDeque(Vector2 element, deque<Vector2> deque) //will compare food to all eel parts for overlap
{
	for (unsigned int i = 0; i < deque.size(); i++) //runs though eel body
	{
		if (Vector2Equals(deque[i], element)) //do body and food share coords?
		{
			return true;
		}
	}
	return false;
}

class Food
{
public: 
	Vector2 position;
	Texture2D texture;

	Food(deque<Vector2> eelBody)
	{
		Image image = LoadImage("CSE350Graphics/tempFood.png");
		texture = LoadTextureFromImage(image);
		UnloadImage(image);
		position = GenerateRandomPos(eelBody);
	}

	~Food()
	{
		UnloadTexture(texture);
	}

	void Draw()
	{
		DrawTexture(texture, position.x * cellSize, position.y * cellSize, WHITE); //texture, x (top left), y (top left), tint
	}

	Vector2 RandomCords()
	{
		float x = GetRandomValue(0, cellCount - 1);
		float y = GetRandomValue(0, cellCount - 1);
		return Vector2{ x, y };
	}

	Vector2 GenerateRandomPos(deque<Vector2> eelBody) 
	{
		Vector2 position = RandomCords();
		while (FoodInDeque(position, eelBody)) //checks for overlap in food and eel body
		{
			position = RandomCords();
		}
		return position;
	}
};

class Eel
{
public:
	deque<Vector2> body = { Vector2{float(cellCount/2), float(cellCount / 2)} }; //a queue that contains cords for eels body
	Vector2 direction = { 1, 0 };
	bool grow = false;

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
		body.push_front(Vector2Add(body[0], direction)); //this will move the eel's head in the direction specified through vector addition
		if (grow == true) 
		{
			grow = false;
		}
		else
		{
			body.pop_back();
		}
	}
	void ResetEel()
	{
		body = { Vector2{float(cellCount / 2), float(cellCount / 2)} };
		direction = { 1, 0 };
	}
};

class Game 
{
public: 
	Eel eel = Eel();
	Food food = Food(eel.body);
	bool ActiveGame = false; //changed to false so there is not immediate start to game

	void Draw()
	{
		food.Draw();
		eel.Draw();
	}
	void Update()
	{
		if (ActiveGame)
		{
			eel.Update();
			CheckForFood();
			CheckForWallCollision();
			CheckForCanibalism();
		}
	}
	void CheckForFood()
	{
		if (Vector2Equals(eel.body[0], food.position))
		{//checks vectors for equality
			food.position = food.GenerateRandomPos(eel.body); // regenerates food
			eel.grow = true;
		}
	}
	void CheckForWallCollision() {
		if (eel.body[0].x == cellCount || eel.body[0].x == -1) //checks for crash on x axis
		{
			GameOver();
		}
		if (eel.body[0].y == cellCount || eel.body[0].y == -1)
		{
			GameOver();
		}
	}
	void CheckForCanibalism()
	{
		deque<Vector2> headlessBody = eel.body; //copy body, than remove head
		headlessBody.pop_front();
		if (FoodInDeque(eel.body[0], headlessBody))
		{
			GameOver();
		}
	}
	void GameOver()
	{
		eel.ResetEel(); //moves snake body to center again
		food.position = food.GenerateRandomPos(eel.body); //resets food location, sends in body to make sure no overlap
		ActiveGame = false;
	}
};

int main()
{
	cout << "Eel..." << endl;
	InitWindow(cellSize*cellCount, cellSize*cellCount, "Eel");
	SetTargetFPS(60);

	Game game = Game();

	while (WindowShouldClose() == false)
	{
		BeginDrawing();
		// add code that if any key is clicked game starts instead of just wasd
		if (timeChecker(0.2)) //delays game loop
		{
			game.Update();
		}

		if (IsKeyPressed(KEY_W) && game.eel.direction.y != 1) //up
		{
			game.eel.direction = { 0, -1 };
			game.ActiveGame = true;
		}
		if (IsKeyPressed(KEY_A) && game.eel.direction.x != 1) //left
		{
			game.eel.direction = { -1, 0 };
			game.ActiveGame = true;
		}
		if (IsKeyPressed(KEY_S) && game.eel.direction.y != -1) //down
		{
			game.eel.direction = { 0, 1 };
			game.ActiveGame = true;
		}
		if (IsKeyPressed(KEY_D) && game.eel.direction.x != -1) //right
		{
			game.eel.direction = { 1, 0 };
			game.ActiveGame = true;
		}
		//Drawing
		ClearBackground(blue);
		game.Draw();

		EndDrawing();
	}

	CloseWindow();
	return 0;
}
