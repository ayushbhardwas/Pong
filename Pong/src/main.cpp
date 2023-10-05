#include "raylib.h"
#include "game.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define TITLE "Pong"
#define TITLE_FONT_SIZE 180

enum ProgramState {
	Menu,
	Game,
	End
};

ProgramState programState = Menu;
GameState gameState;

void Update();
void Draw();

int main() {

	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, TITLE);

	while (!WindowShouldClose()) {

		Update();
		Draw();
	}

	CloseWindow();
}

void Update() {

	switch (programState) {
	case Menu:
	{
		if (IsKeyPressed(KEY_ENTER)) {
			InitializeGame(WINDOW_WIDTH, WINDOW_HEIGHT);
			programState = Game;
		}
	}break;
	case Game:
	{
		UpdateGame(WINDOW_WIDTH, WINDOW_HEIGHT);
		if (GetGameState() != Unknown) {
			programState = End;
			gameState = GetGameState();
		}
	}break;
	case End:
	{
		if (IsKeyPressed(KEY_ENTER)) {
			InitializeGame(WINDOW_WIDTH, WINDOW_HEIGHT);
			programState = Game;
		}
	}break;
	}

}

void Draw() {

	BeginDrawing();
	ClearBackground(BLACK);

	switch (programState) {
	case Menu:
	{
		DrawText(TITLE, (WINDOW_WIDTH / 2) - (MeasureText(TITLE, TITLE_FONT_SIZE) / 2), WINDOW_HEIGHT/9, TITLE_FONT_SIZE, RAYWHITE);
		DrawText("Press 'Enter' to begin", (WINDOW_WIDTH / 2) - (MeasureText("Press 'Enter' to begin", 30) / 2), 3 * (WINDOW_HEIGHT / 4), 30, DARKGRAY);

	}break;
	case Game:
	{
		DrawGame(WINDOW_WIDTH, WINDOW_HEIGHT);

	}break;
	case End:
	{
		if (gameState == Win) {
			DrawText("You Win", (WINDOW_WIDTH / 2) - (MeasureText("You Win", 80) / 2), (WINDOW_HEIGHT / 2) - 80, 80, RAYWHITE);
		}
		else if (gameState == Lose) {
			DrawText("You Lose", (WINDOW_WIDTH / 2) - (MeasureText("You Lose", 80) / 2), (WINDOW_HEIGHT / 2) - 80, 80, RAYWHITE);
		}

		DrawText("Press 'Enter' to play again", (WINDOW_WIDTH / 2) - (MeasureText("Press 'Enter' to play again", 30) / 2), 3 * (WINDOW_HEIGHT / 4), 30, DARKGRAY);

	}break;
	}

	EndDrawing();
}