#include "Game.h"
#include "Player.h"
#include "Map.h"
#include "Bomb.h"
#include "Explosion.h"

#include <iostream>
#include <vector>
#include "raylib.h"

using namespace std;

vector<Explosion> explosions;

Game::Game()
{
	InitWindow(1600, 900, "Bomberman - Eldiablo");
	SetTargetFPS(60);

	InitAudioDevice();
	music = LoadMusicStream("resources/bombermanAudio/music/StageTheme.wav");
	SetMusicVolume(music, 0.2f);
	PlayMusicStream(music);

	camera.target = { 0, 0 };
	camera.offset = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
	camera.rotation = 0;
	camera.zoom = 2;
}

Game::~Game()
{
	CloseWindow();
}

void Game::Run()
{
	Player player;
	Map map;

	// Estados del juego
	enum GameState { SPLASH, TITLE, GAMEPLAY };
	GameState currentState = SPLASH;

	// Recursos visuales
	Texture2D titleScreen = LoadTexture("resources/bombermanSprites/UI/NES - Bomberman - Title Screen & Text.png");
	Font font = GetFontDefault();

	while (!WindowShouldClose())
	{
		UpdateMusicStream(music);
		BeginDrawing();
		ClearBackground(LIGHTGRAY);

		switch (currentState)
		{
		case SPLASH:
			DrawText("This is a recreation of Bomberman NES. Press ENTER to continue.", 300, 400, 30, BLACK);
			DrawText("Proyecto I, Disseny i desenvolupament de videojocs, CITM Terrassa.", 400, 500, 15, BLACK);
			DrawText("Pol Cuenca, Andrea Velez, Daniel Castillero. Tutor: Alejandro Paris Gomez ", 400, 600, 15, BLACK);
			if (IsKeyPressed(KEY_ENTER)) currentState = TITLE;
			break;

		case TITLE:
			DrawTexture(titleScreen, 0, 0, BLACK);
			DrawText("Presiona ENTER para comenzar", 500, 850, 25, BLACK);
			if (IsKeyPressed(KEY_ENTER)) currentState = GAMEPLAY;
			break;

		case GAMEPLAY:
			player.UpdatePlayer(map);
			Vector2 targetPos = player.GetPlayerPos();
			camera.target = targetPos;

			BeginMode2D(camera);
			map.DrawMap();
			player.DrawPlayer();
			EndMode2D();
			break;
		}

		EndDrawing();
	}

	UnloadTexture(titleScreen);
}
