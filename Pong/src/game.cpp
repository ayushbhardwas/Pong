#include "game.h"
#include "raylib.h"

#define BALL_RADIUS 20
#define BALL_SPEED 320
#define PADDLE_WIDTH 40
#define PADDLE_HEIGHT 80
#define PADDLE_PADDING 40
#define PADDLE_SPEED 220
#define WINNING_SCORE 5
#define SCORE_FONT_SIZE 50

const int BOT_SPEED_VARIATION_MIN = -12;
const int BOT_SPEED_VARIATION_MAX = 80;

Vector2 ballPosition;
Vector2 ballDirection;
Rectangle botPaddle;
Rectangle playerPaddle;

int botScore;
int playerScore;

void InitializeGame(int windowWidth, int windowHeight) {

	//ball-------------------------------------------------------------------------
	ballPosition = { (float)windowWidth / 2, (float)windowHeight / 2 };
	int randomX = GetRandomValue(0, 1) ? 1 : -1;
	int randomY = GetRandomValue(0, 1) ? 1 : -1;
	ballDirection = { randomX * 0.8f, randomY * 0.2f };
	//-----------------------------------------------------------------------------

	//paddles----------------------------------------------------------------------
	botPaddle = { PADDLE_PADDING, (float)windowHeight / 2 - PADDLE_HEIGHT / 2, PADDLE_WIDTH, PADDLE_HEIGHT };
	playerPaddle = { (float)windowWidth - PADDLE_WIDTH - PADDLE_PADDING, (float)windowHeight / 2 - PADDLE_HEIGHT / 2, PADDLE_WIDTH, PADDLE_HEIGHT };
	//-----------------------------------------------------------------------------

	//scores-----------------------------------------------------------------------
	if (botScore == WINNING_SCORE || playerScore == WINNING_SCORE) {
		botScore = 0;
		playerScore = 0;
	}
	//-----------------------------------------------------------------------------

}


void UpdateGame(int windowWidth, int windowHeight) {

	//paddle movement--------------------------------------------------------------

	//bot paddle
	if (botPaddle.y + (PADDLE_HEIGHT / 2) > ballPosition.y && botPaddle.y > 0) {
		botPaddle.y -= (PADDLE_SPEED + GetRandomValue(BOT_SPEED_VARIATION_MIN, BOT_SPEED_VARIATION_MAX)) * GetFrameTime();
	}
	else if (botPaddle.y + (PADDLE_HEIGHT / 2) < ballPosition.y && botPaddle.y + PADDLE_HEIGHT < windowHeight) {
		botPaddle.y += (PADDLE_SPEED + GetRandomValue(BOT_SPEED_VARIATION_MIN, BOT_SPEED_VARIATION_MAX)) * GetFrameTime();
	}

	//player paddle
	if (IsKeyDown(KEY_UP) && playerPaddle.y > 0) {
		playerPaddle.y -= PADDLE_SPEED * GetFrameTime();
	}
	else if (IsKeyDown(KEY_DOWN) && playerPaddle.y + PADDLE_HEIGHT < windowHeight) {
		playerPaddle.y += PADDLE_SPEED * GetFrameTime();
	}

	//-----------------------------------------------------------------------------

	//ball-paddle collision--------------------------------------------------------

	//bot paddle
	if (CheckCollisionCircleRec(ballPosition, BALL_RADIUS, botPaddle)) {
		
		//increase ball's velocity-----------------------------------------------------
		if (ballDirection.x > -1 && ballDirection.x < 1) {
			ballDirection.x += ballDirection.x > 0 ? 0.1 : -0.1;
		}
		if (ballDirection.y > -1 && ballDirection.y < 1) {
			ballDirection.y += ballDirection.y > 0 ? 0.1 : -0.1;
		}
		//-----------------------------------------------------------------------------

		if (ballPosition.x > botPaddle.x + PADDLE_WIDTH) {
			ballDirection.x *= -1;
			ballPosition.x += (botPaddle.x + PADDLE_WIDTH) - (ballPosition.x - BALL_RADIUS);
		}

		if (ballPosition.y + (BALL_RADIUS * 0.9) < botPaddle.y) {
			ballDirection.y *= -1;
			ballPosition.y -= (ballPosition.y + BALL_RADIUS) - botPaddle.y;
		}
		else if (ballPosition.y - (BALL_RADIUS * 0.9) > botPaddle.y + PADDLE_HEIGHT) {
			ballDirection.y *= -1;
			ballPosition.y += (botPaddle.y + PADDLE_HEIGHT) - (ballPosition.y - BALL_RADIUS);
		}

	}

	//player paddle
	if (CheckCollisionCircleRec(ballPosition, BALL_RADIUS, playerPaddle)) {

		//increase ball's velocity-----------------------------------------------------
		if (ballDirection.x > -1 && ballDirection.x < 1) {
			ballDirection.x += ballDirection.x > 0 ? 0.1 : -0.1;
		}
		if (ballDirection.y > -1 && ballDirection.y < 1) {
			ballDirection.y += ballDirection.y > 0 ? 0.1 : -0.1;
		}
		//-----------------------------------------------------------------------------

		if (ballPosition.x < playerPaddle.x) {
			ballDirection.x *= -1;
			ballPosition.x -= (ballPosition.x + BALL_RADIUS) - (playerPaddle.x);
		}

		if (ballPosition.y < playerPaddle.y) {
			ballDirection.y *= -1;
			ballPosition.y -= (ballPosition.y + BALL_RADIUS) - playerPaddle.y;
		}
		else if (ballPosition.y > playerPaddle.y + PADDLE_HEIGHT) {
			ballDirection.y *= -1;
			ballPosition.y += (playerPaddle.y + PADDLE_HEIGHT) - (ballPosition.y - BALL_RADIUS);
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
		playerScore++;
		if (playerScore != WINNING_SCORE) {
			InitializeGame(windowWidth, windowHeight);
		}
	}
	else if (ballPosition.x - BALL_RADIUS > windowWidth) {
		botScore++;
		if (botScore != WINNING_SCORE) {
			InitializeGame(windowWidth, windowHeight);
		}
	}
	//-----------------------------------------------------------------------------
}

void DrawGame(int windowWidth, int windowHeight) {
	//background elements----------------------------------------------------------
	DrawLine(windowWidth / 2, 0, windowWidth / 2, windowHeight, DARKGRAY);
	DrawCircleLines(windowWidth / 2, windowHeight / 2, 110, DARKGRAY);
	//-----------------------------------------------------------------------------

	//score------------------------------------------------------------------------
	const char* botScoreText = TextFormat("BOT: %i", botScore);
	const char* playerScoreText = TextFormat("YOU: %i", playerScore);
	DrawText(botScoreText, (2 * (windowWidth / 8)) - (MeasureText(botScoreText, SCORE_FONT_SIZE) / 2), windowHeight / 10, SCORE_FONT_SIZE, DARKGRAY);
	DrawText(playerScoreText, (6 * (windowWidth / 8)) - (MeasureText(playerScoreText, SCORE_FONT_SIZE) / 2), windowHeight / 10, SCORE_FONT_SIZE, DARKGRAY);
	//-----------------------------------------------------------------------------

	//game elements----------------------------------------------------------------
	DrawCircle(ballPosition.x, ballPosition.y, BALL_RADIUS, RAYWHITE);
	DrawRectangleRec(botPaddle, RAYWHITE);
	DrawRectangleRec(playerPaddle, RAYWHITE);
	//-----------------------------------------------------------------------------
}

GameState GetGameState() {
	if (playerScore == WINNING_SCORE) {
		return Win;
	}
	else if (botScore == WINNING_SCORE) {
		return Lose;
	}
	else {
		return Unknown;
	}
}