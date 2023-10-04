#pragma once

enum GameState {
	Unknown,
	Win,
	Lose
};

void InitializeGame(int windowWidth, int windowHeight);
void UpdateGame(int windowWidth, int windowHeight);
void DrawGame(int windowWidth, int windowHeight);

GameState GetGameState();