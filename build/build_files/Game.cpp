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



    enum GameState { SPLASH, TITLE, GAMEPLAY };
    GameState currentState = SPLASH;
    Texture2D titleScreen = LoadTexture("resources/bombermanSprites/UI/NES - Bomberman - Title Screen & Text.png");

        while (!WindowShouldClose())
        {
            UpdateMusicStream(music);
            BeginDrawing();
            ClearBackground(BLACK);

            switch (currentState)
            {
            case SPLASH:
                DrawText("This is a recreation of Bomberman NES. Press ENTER to continue.", 100, 350, 40, WHITE);
                DrawText("Proyecto I, Disseny i desenvolupament de videojocs, CITM Terrassa.", 200, 450, 30, WHITE);
                DrawText("Pol Cuenca, Andrea Velez, Daniel Castillero. Tutor: Alejandro Paris Gomez ", 200, 500, 30, WHITE);
                if (IsKeyPressed(KEY_ENTER)) currentState = TITLE;
                break;

            case TITLE:
                DrawTextureEx(titleScreen,{ 350, 0 }, 0, 3.5, WHITE);
                DrawText("Presiona ENTER para comenzar", 500, 850, 25, WHITE);
                if (IsKeyPressed(KEY_ENTER)) currentState = GAMEPLAY;
                break;

            case GAMEPLAY:

                player.UpdatePlayer(map);
                Vector2 targetPos = player.GetPlayerPos();
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

                for (const Explosion& e : explosions)
                {
                    e.DrawExplosion();

                    if (CheckCollisionRecs(player.GetPlayerRect(), e.GetExplosionRect()))
                    {
                        player.TakeDamage();

                        if (player.GetCurrentHp() <= 0)
                        {
                            // Pantalla de game over
                            CloseWindow();
                        }
                        else
                        {
                            player.SetPlayerPos(); // cambiar a reset player pos
                            // resetear mapa
                            explosions.clear();
                        }

                        break;
                    }
                }

                if (IsKeyReleased(KEY_P))
                {
                    player.SetPlayerPos();
                    player.TakeDamage();
                    //cout << player.GetCurrentHp() << endl;
                    //map.ClearMap();
                }

                if (player.GetCurrentHp() <= 0)
                {
                    CloseWindow();
                }

                EndMode2D();
                break;
            }

            EndDrawing();
        }

        UnloadTexture(titleScreen);
        UnloadMusicStream(music);
        CloseAudioDevice();
        CloseWindow();
    
}