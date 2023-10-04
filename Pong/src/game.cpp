#include "game.h"
#include "raylib.h"
#include <cmath>

#define BALL_RADIUS 20
#define BALL_SPEED 320
#define PADDLE_WIDTH 40
#define PADDLE_HEIGHT 80
#define PADDLE_PADDING 40
#define PADDLE_SPEED 220
#define SCORE_FONT_SIZE 50

const int BOT_PADDLE_MIN_SPEED = -std::round(BALL_SPEED / 30);
const int BOT_PADDLE_MAX_SPEED = std::round(BALL_SPEED / 4);

Vector2 ballPosition;
Vector2 ballDirection;
Rectangle leftPaddle;
Rectangle rightPaddle;
int leftScore = 0;
int rightScore = 0;

void InitializeGame(int windowWidth, int windowHeight) {

	//ball-------------------------------------------------------------------------
	ballPosition = { (float)windowWidth / 2, (float)windowHeight / 2 };
	int randomX = GetRandomValue(0, 1) ? 1 : -1;
	int randomY = GetRandomValue(0, 1) ? 1 : -1;
	ballDirection = { randomX * 0.8f, randomY * 0.2f };
	//-----------------------------------------------------------------------------

	//paddles----------------------------------------------------------------------
	leftPaddle = { PADDLE_PADDING, (float)windowHeight / 2 - PADDLE_HEIGHT / 2, PADDLE_WIDTH, PADDLE_HEIGHT };
	rightPaddle = { (float)windowWidth - PADDLE_WIDTH - PADDLE_PADDING, (float)windowHeight / 2 - PADDLE_HEIGHT / 2, PADDLE_WIDTH, PADDLE_HEIGHT };
	//-----------------------------------------------------------------------------

}


void UpdateGame(int windowWidth, int windowHeight) {

	//paddle control---------------------------------------------------------------

	//left paddle
	if (leftPaddle.y + (PADDLE_HEIGHT / 2) > ballPosition.y && leftPaddle.y > 0) {
		leftPaddle.y -= (PADDLE_SPEED + GetRandomValue(BOT_PADDLE_MIN_SPEED, BOT_PADDLE_MAX_SPEED)) * GetFrameTime();
	}
	else if (leftPaddle.y + (PADDLE_HEIGHT / 2) < ballPosition.y && leftPaddle.y + PADDLE_HEIGHT < windowHeight) {
		leftPaddle.y += (PADDLE_SPEED + GetRandomValue(BOT_PADDLE_MIN_SPEED, BOT_PADDLE_MAX_SPEED)) * GetFrameTime();
	}

	//right paddle
	if (IsKeyDown(KEY_UP) && rightPaddle.y > 0) {
		rightPaddle.y -= PADDLE_SPEED * GetFrameTime();
	}
	else if (IsKeyDown(KEY_DOWN) && rightPaddle.y + PADDLE_HEIGHT < windowHeight) {
		rightPaddle.y += PADDLE_SPEED * GetFrameTime();
	}
	//-----------------------------------------------------------------------------

	//ball-paddle collision--------------------------------------------------------

	//left paddle
	if (CheckCollisionCircleRec(ballPosition, BALL_RADIUS, leftPaddle)) {
		
		//increase ball's velocity-----------------------------------------------------
		if (ballDirection.x > -1 && ballDirection.x < 1) {
			ballDirection.x += ballDirection.x > 0 ? 0.1 : -0.1;
		}
		if (ballDirection.y > -1 && ballDirection.y < 1) {
			ballDirection.y += ballDirection.y > 0 ? 0.1 : -0.1;
		}
		//-----------------------------------------------------------------------------

		if (ballPosition.x > leftPaddle.x + PADDLE_WIDTH) {
			ballDirection.x *= -1;
			ballPosition.x += (leftPaddle.x + PADDLE_WIDTH) - (ballPosition.x - BALL_RADIUS);
		}

		if (ballPosition.y + (BALL_RADIUS * 0.9) < leftPaddle.y) {
			ballDirection.y *= -1;
			ballPosition.y -= (ballPosition.y + BALL_RADIUS) - leftPaddle.y;
		}
		else if (ballPosition.y - (BALL_RADIUS * 0.9) > leftPaddle.y + PADDLE_HEIGHT) {
			ballDirection.y *= -1;
			ballPosition.y += (leftPaddle.y + PADDLE_HEIGHT) - (ballPosition.y - BALL_RADIUS);
		}

	}

	//right paddle
	if (CheckCollisionCircleRec(ballPosition, BALL_RADIUS, rightPaddle)) {

		//increase ball's velocity-----------------------------------------------------
		if (ballDirection.x > -1 && ballDirection.x < 1) {
			ballDirection.x += ballDirection.x > 0 ? 0.1 : -0.1;
		}
		if (ballDirection.y > -1 && ballDirection.y < 1) {
			ballDirection.y += ballDirection.y > 0 ? 0.1 : -0.1;
		}
		//-----------------------------------------------------------------------------

		if (ballPosition.x < rightPaddle.x) {
			ballDirection.x *= -1;
			ballPosition.x -= (ballPosition.x + BALL_RADIUS) - (rightPaddle.x);
		}

		if (ballPosition.y < rightPaddle.y) {
			ballDirection.y *= -1;
			ballPosition.y -= (ballPosition.y + BALL_RADIUS) - rightPaddle.y;
		}
		else if (ballPosition.y > rightPaddle.y + PADDLE_HEIGHT) {
			ballDirection.y *= -1;
			ballPosition.y += (rightPaddle.y + PADDLE_HEIGHT) - (ballPosition.y - BALL_RADIUS);
		}

	}
	//-----------------------------------------------------------------------------

	//ball-bounday collision-------------------------------------------------------
	if (ballPosition.y - BALL_RADIUS < 0) {
		ballDirection.y *= -1;
		ballPosition.y += 0 - (ballPosition.y - BALL_RADIUS);
	}
	else if (ballPosition.y + BALL_RADIUS > windowHeight) {
		ballDirection.y *= -1;
		ballPosition.y -= (ballPosition.y + BALL_RADIUS) - windowHeight;
	}
	//-----------------------------------------------------------------------------

	//ball movement----------------------------------------------------------------
	ballPosition.x += ballDirection.x * BALL_SPEED * GetFrameTime();
	ballPosition.y += ballDirection.y * BALL_SPEED * GetFrameTime();
	//-----------------------------------------------------------------------------

	//point scored-----------------------------------------------------------------
	if (ballPosition.x + BALL_RADIUS < 0) {
		rightScore++;
		InitializeGame(windowWidth, windowHeight);
	}
	else if (ballPosition.x - BALL_RADIUS > windowWidth) {
		leftScore++;
		InitializeGame(windowWidth, windowHeight);
	}
	//-----------------------------------------------------------------------------
}

void DrawGame(int windowWidth, int windowHeight) {
	//background elements----------------------------------------------------------
	DrawLine(windowWidth / 2, 0, windowWidth / 2, windowHeight, DARKGRAY);
	DrawCircleLines(windowWidth / 2, windowHeight / 2, 110, DARKGRAY);
	//-----------------------------------------------------------------------------

	//score------------------------------------------------------------------------
	const char* leftScoreText = TextFormat("BOT: %i", leftScore);
	const char* rightScoreText = TextFormat("YOU: %i", rightScore);
	DrawText(leftScoreText, (2 * (windowWidth / 8)) - (MeasureText(leftScoreText, SCORE_FONT_SIZE) / 2), windowHeight / 10, SCORE_FONT_SIZE, DARKGRAY);
	DrawText(rightScoreText, (6 * (windowWidth / 8)) - (MeasureText(rightScoreText, SCORE_FONT_SIZE) / 2), windowHeight / 10, SCORE_FONT_SIZE, DARKGRAY);
	//-----------------------------------------------------------------------------

	//game elements----------------------------------------------------------------
	DrawCircle(ballPosition.x, ballPosition.y, BALL_RADIUS, RAYWHITE);
	DrawRectangleRec(leftPaddle, RAYWHITE);
	DrawRectangleRec(rightPaddle, RAYWHITE);
	//-----------------------------------------------------------------------------
}