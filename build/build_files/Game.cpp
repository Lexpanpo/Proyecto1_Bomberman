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
}

Game::~Game()
{
	CloseWindow;
}


void Game::Run()
{
	Player player;
	Map map;

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(BLACK);

		map.DrawMap();
		player.UpdatePlayer(map);
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

		EndDrawing();
	}
}