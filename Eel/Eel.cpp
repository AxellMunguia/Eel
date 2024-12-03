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
Color green = { 0, 255, 0, 255 };

int cellSize = 30;
int cellCount = 25;
int offset = 75;

double startTime = 0;
double startBombTime = 0;
double startFrogTime = 0;
double startCoinTime = 0;

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
bool bombClock(double interval)
{
	double currentTime = GetTime();
	if (currentTime - startBombTime >= interval)
	{
		startBombTime = currentTime;
		return true; //yes, interval has elapsed
	}
	return false; //no, interval has not elapsed
}
bool frogClock(double interval)
{
	double currentTime = GetTime();
	if (currentTime - startFrogTime >= interval)
	{
		startFrogTime = currentTime;
		return true; //yes, interval has elapsed
	}
	return false; //no, interval has not elapsed
}
bool coinClock(double interval)
{
	double currentTime = GetTime();
	if (currentTime - startCoinTime >= interval)
	{
		startCoinTime = currentTime;
		return true; //yes, interval has elapsed
	}
	return false; // no, interval has not elapsed
}

bool ElementInDeque(Vector2 element, deque<Vector2> deque) //will compare food to all eel parts for overlap
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

class Bomb 
{
public: 
	Vector2 position;
	Texture2D texture;

	Bomb(deque <Vector2> eelBody)
	{
		Image image = LoadImage("CSE350Graphics/bomb.png");
		texture = LoadTextureFromImage(image);
		UnloadImage(image);
		position = GenerateRandomPos(eelBody);
	}

	~Bomb()
	{
		UnloadTexture(texture);
	}
	
	void Draw()
	{
		DrawTexture(texture, offset + position.x * cellSize, offset + position.y * cellSize, WHITE); //texture, x (top left), y (top left), tint
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
		while (ElementInDeque(position, eelBody)) //checks for overlap in food and eel body
		{
			position = RandomCords();
		}
		return position;
	}
};

class Food
{
public: 
	Vector2 position;
	Texture2D texture;

	Food(deque<Vector2> eelBody)
	{
		Image image = LoadImage("CSE350Graphics/fish.png");
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
		DrawTexture(texture, offset + position.x * cellSize, offset + position.y * cellSize, WHITE); //texture, x (top left), y (top left), tint
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
		while (ElementInDeque(position, eelBody)) //checks for overlap in food and eel body
		{
			position = RandomCords();
		}
		return position;
	}
};

class FrogFood
{
public:
	Vector2 position;
	Texture2D texture;

	FrogFood(deque <Vector2> eelBody)
	{
		Image image = LoadImage("CSE350Graphics/frog.png");
		texture = LoadTextureFromImage(image);
		UnloadImage(image);
		position = GenerateRandomPos(eelBody);
	}

	~FrogFood()
	{
		UnloadTexture(texture);
	}

	void Draw()
	{
		DrawTexture(texture, offset + position.x * cellSize, offset + position.y * cellSize, WHITE);
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
		while (ElementInDeque(position, eelBody))
		{
			position = RandomCords();
		}
		return position;
	}
};

class Coin
{
public:
	Vector2 position;
	Texture2D texture;

	Coin(deque <Vector2> eelBody)
	{
		Image image = LoadImage("CSE350Graphics/coin.png");
		texture = LoadTextureFromImage(image);
		UnloadImage(image);
		position = GenerateRandomPos(eelBody);
	}

	~Coin()
	{
		UnloadTexture(texture);
	}

	void Draw()
	{
		DrawTexture(texture, offset + position.x * cellSize, offset + position.y * cellSize, WHITE);
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
		while (ElementInDeque(position, eelBody))
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
			DrawRectangle(offset + x * cellSize, offset + y * cellSize, cellSize, cellSize, green); //x (top left), y (top left), width, height, color
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
	Bomb bomb = Bomb(eel.body);
	FrogFood frog = FrogFood(eel.body);
	Coin coin = Coin(eel.body);

	bool ActiveGame = false; //changed to false so there is not immediate start to game
	bool DrawBomb = false;
	bool DrawFrogFood = false;
	bool DrawCoin = false;

	int score = 0;

	void Draw()
	{
		food.Draw();
		eel.Draw();
		if (DrawBomb)
		{
			bomb.Draw();
		}
		if (DrawFrogFood)
		{
			frog.Draw();
		}
		if (DrawCoin)
		{
			coin.Draw();
		}
	}
	void Update()
	{
		if (ActiveGame)
		{
			eel.Update();
			CheckForFood();
			CheckForWallCollision();
			CheckForCanibalism();
			CheckForBombCollison();
		}
	}
	void CheckForFood()
	{
		if (Vector2Equals(eel.body[0], food.position))
		{//checks vectors for equality
			food.position = food.GenerateRandomPos(eel.body); // regenerates food
			eel.grow = true;
			score++;
		}

		if (Vector2Equals(eel.body[0], frog.position))
		{
			frog.position = { -10, -10 };
			eel.grow = true;
			score = score + 2;

		}

		if (Vector2Equals(eel.body[0], coin.position))
		{
			coin.position = { -10, -10 };
			score = score + 5;
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
		if (ElementInDeque(eel.body[0], headlessBody))
		{
			GameOver();
		}
	}
	void GameOver()
	{
		eel.ResetEel(); //moves snake body to center again
		food.position = food.GenerateRandomPos(eel.body); //resets food location, sends in body to make sure no overlap
		bomb.position = { -1, -1 }; // regenerates bomb
		frog.position = { -1, -1 };
		coin.position = { -1, -1 };
		ActiveGame = false; //pause game till start
		score = 0;

	}
	void CheckForBombCollison()
	{
		if (Vector2Equals(eel.body[0], bomb.position))
		{	
			GameOver();
		}
	}
	
	void DrawGrid()
	{
		for (int i = 0; i < cellCount; i++)
		{
			for (int j = 0; j < cellCount; j++)
			{
				DrawRectangleLines(offset + i * cellSize, offset + j * cellSize, cellSize, cellSize, WHITE);
			}
		}
	}
};

int main()
{
	cout << "Eel..." << endl;
	InitWindow(cellSize * cellCount + 2 * offset, cellSize * cellCount + 2 * offset, "Eel");
	SetTargetFPS(60);

	Game game = Game();

	while (WindowShouldClose() == false)
	{
		BeginDrawing();
		// add code that if any key is clicked game starts instead of just wasd
		if (game.ActiveGame) // if game is running start timer for bomb, 
		{
			if (bombClock(10))
			{
				if (game.DrawBomb == false) 
				{
					game.DrawBomb = true;
					//cout << "10 secs has elapsed, eel should be drawn" << endl;
					if (Vector2Equals(game.bomb.position, { -1, -1 }))
					{
						game.bomb.position = game.bomb.GenerateRandomPos(game.eel.body);
					}
				}
				else if (game.DrawBomb == true)
				{
					game.DrawBomb = false;
					game.bomb.position = { -1, -1 }; //moves bomb off map
					//cout << "10 secs has elapsed, eel should not be drawn" << endl;
				}
			}

			if (frogClock(15))
			{
				if (game.DrawFrogFood == false)
				{
					game.DrawFrogFood = true;
					if (Vector2Equals(game.frog.position, { -1, -1 }))
					{
						game.frog.position = game.frog.GenerateRandomPos(game.eel.body);
					}
				}
				else if (game.DrawFrogFood == true)
				{
					game.DrawFrogFood = false;
					game.frog.position = { -1, -1 }; // moves frog off map
				}
			}

			if (coinClock(5))
			{
				if (game.DrawCoin == false)
				{
					game.DrawCoin = true;
					if (Vector2Equals(game.coin.position, { -1, -1 }))
					{
						game.coin.position = game.coin.GenerateRandomPos(game.eel.body);
					}
				}
				else if (game.DrawCoin == true)
				{
					game.DrawCoin = false;
					game.coin.position = { -1, -1 }; // moves coin off map
				}
			}
		}
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
		DrawRectangleLinesEx(Rectangle{ (float)offset - 5, (float)offset - 5, (float)cellSize * cellCount + 10, (float)cellSize * cellCount + 10 }, 5, WHITE);
		DrawText("Eel: A Reimgination of Snake", offset - 5, 20, 40, WHITE); //text, x, y, font size, color
		DrawText(TextFormat("%i", game.score), offset - 5, offset + cellSize * cellCount + 10, 30, WHITE);
		game.DrawGrid();
		game.Draw();
		EndDrawing();
	}

	CloseWindow();
	return 0;
}
