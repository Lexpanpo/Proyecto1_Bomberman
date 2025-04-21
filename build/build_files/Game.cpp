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

	while (!WindowShouldClose())
	{
		UpdateMusicStream(music);

		player.UpdatePlayer(map);

		
		Vector2 targetPos = player.GetPlayerPos();
		//cout << "PosX: " << targetPos.x << endl;
		//cout << "PosY: " << targetPos.y << endl;

		camera.target = targetPos;

		float mapWidth = 31 * 40;
		float mapHeight = 13 * 40;
		float halfWidth = GetScreenWidth() / (2.0f * camera.zoom);
		float halfHeight = GetScreenHeight() / (2.0f * camera.zoom);

		if (camera.target.x < halfWidth)
		{
			camera.target.x = halfWidth;
		}
		if (camera.target.y < halfHeight)
		{
			camera.target.y = halfHeight;
		}
		if (camera.target.x > mapWidth - halfWidth)
		{
			camera.target.x = mapWidth - halfWidth;
		}
		if (camera.target.y > mapHeight - halfHeight)
		{
			camera.target.y = mapHeight - halfHeight;
		}

		BeginDrawing();
		ClearBackground(BLACK);

		BeginMode2D(camera);

		map.DrawMap();
		player.DrawPlayer();

		for (int i = 0; i < explosions.size();)
		{
			if (explosions[i].UpdateStatus(GetFrameTime())) 
			{
				explosions.erase(explosions.begin() + i);
			}
			else
			{
				i++;
			}
		}

		for (const Explosion& e : explosions) e.DrawExplosion();

		EndMode2D();

		EndDrawing();
	}

	UnloadMusicStream(music);
	CloseAudioDevice();
	CloseWindow();
}