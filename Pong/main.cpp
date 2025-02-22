#include <iostream>
#include <raylib.h>


struct Ball
{
	float x, y;
	float speedX, speedY;
	float radius;

	void Draw()
	{
		(DrawCircle((int)x, (int)y, radius, WHITE));
	}
	void Reset()
	{
		speedX = 300;
		speedY = 300;
	}

};

struct Paddle
{
	float x, y;
	float speed;
	float width, height;

	Rectangle GetRect()
	{
		return Rectangle{ x - width / 2, y - height / 2, 10, 100 };
	}

	void Draw() 
	{
		DrawRectangleRec(GetRect(), WHITE);
	}

};

struct PlayerScore
{
	int score = 0;
	int maxScore = 10;

	void Draw(int x, int y)
	{
		DrawText(TextFormat("%d", score), x, y, 60, WHITE);
	}
};

void EndGame(const char*& winnerText, const char* winnerMessage, Ball& ball)
{
	ball.speedX = 0;
	ball.speedY = 0;
	winnerText = winnerMessage;
}

int main() {

	InitWindow(800, 600, "Pong");
	SetWindowState(FLAG_VSYNC_HINT);

	PlayerScore player1;
	PlayerScore player2;

	Ball ball;
	ball.x = GetScreenWidth() / 2.0f;
	ball.y = GetScreenHeight() / 2.0f;
	ball.radius = 5;
	ball.speedX = 300;
	ball.speedY = 300;

	Paddle leftPaddle;
	leftPaddle.x = 50;
	leftPaddle.y = GetScreenHeight() / 2;
	leftPaddle.width = 10;
	leftPaddle.height = 100;
	leftPaddle.speed = 500;

	Paddle rightPaddle;
	rightPaddle.x = GetScreenWidth() - 50;
	rightPaddle.y = GetScreenHeight() / 2;
	rightPaddle.width = 10;
	rightPaddle.height = 100;
	rightPaddle.speed = 500;

	const char* winnerText = nullptr;
	const char* playerScore = nullptr;

	while (!WindowShouldClose())
	{
		ball.x += ball.speedX * GetFrameTime();
		ball.y += ball.speedY * GetFrameTime();

		if (ball.y < 0)
		{
			ball.y = 0;
			ball.speedY *= -1;
		}
		if (ball.y > GetScreenHeight())
		{
			ball.y = GetScreenHeight();
			ball.speedY *= -1;
		}

		if (IsKeyDown(KEY_W))
		{
			leftPaddle.y -= leftPaddle.speed * GetFrameTime();
		}
		if (IsKeyDown(KEY_S))
		{
			leftPaddle.y += leftPaddle.speed * GetFrameTime();
		}

		if (IsKeyDown(KEY_UP))
		{
			rightPaddle.y -= rightPaddle.speed * GetFrameTime();
		}
		if (IsKeyDown(KEY_DOWN))
		{
			rightPaddle.y += rightPaddle.speed * GetFrameTime();
		}

		if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius,
			leftPaddle.GetRect()))
		{
			if (ball.speedX < 0)
			{
				ball.speedX *= -1.1f;
				ball.speedY = (ball.y - leftPaddle.y) / (leftPaddle.height / 2) * 
					ball.speedX;
			}
		}
		if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius,
			rightPaddle.GetRect()))
		{
			if (ball.speedX > 0)
			{
				ball.speedX *= -1.1f;
				ball.speedY = (ball.y - rightPaddle.y) / (rightPaddle.height / 2) * -
					ball.speedX;
			}
		}

		

		if (ball.x < 0 && player2.score < 10)
		{
			player2.score++;
			ball.x = GetScreenHeight() / 2.0f;
			ball.y = GetScreenWidth() / 2.0f;
			ball.Reset();
		}
		if (ball.x > GetScreenWidth() && player1.score < 10)
		{
			player1.score++;
			ball.x = GetScreenHeight() / 2.0f;
			ball.y = GetScreenWidth() / 2.0f;
			ball.Reset();
		}
		
		if (player1.score == 10)
		{
			EndGame(winnerText, "Player 1 wins!", ball);
		}
		if (player2.score == 10)
		{
			EndGame(winnerText, "Player 2 wins!", ball);
		}

		BeginDrawing();
		ClearBackground(BLACK);

		ball.Draw();
		leftPaddle.Draw();
		rightPaddle.Draw();
		player1.Draw(10, 10);
		player2.Draw(GetScreenWidth() - 40, 10);

		if (winnerText)
		{
			int textWidth = MeasureText(winnerText, 60);
			DrawText(winnerText, (GetScreenWidth() - textWidth) / 2, 
				GetScreenHeight() / 2 - 30, 60, YELLOW);
		}

		EndDrawing();
	}

	CloseWindow();

}