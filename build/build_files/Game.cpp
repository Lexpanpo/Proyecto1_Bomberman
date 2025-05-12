#include "Game.h"
#include "Player.h"
#include "Map.h"
#include "Bomb.h"
#include "Explosion.h"

#include <iostream>
#include <vector>
#include "raylib.h"
#define SPRITES "resources/bombermanSprites/General Sprites/Player_Sprites.png"

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

    /*fxWav1 = LoadSound("resources/bombermanAudio/SFX/Bomberman SFX (1).wav");
    fxWav2 = LoadSound("resources/bombermanAudio/SFX/Bomberman SFX (2).wav");
    fxWav3 = LoadSound("resources/bombermanAudio/SFX/Bomberman SFX (3).wav");
    fxWav4 = LoadSound("resources/bombermanAudio/SFX/Bomberman SFX (4).wav");
    fxWav5 = LoadSound("resources/bombermanAudio/SFX/Bomberman SFX (5).wav");
    fxWav6 = LoadSound("resources/bombermanAudio/SFX/Bomberman SFX (6).wav");
    fxWav19 = LoadSound("resources/bombermanAudio/SFX/Bomberman II SFX (19).wav");*/

    /*if (IsKeyPressed(KEY_RIGHT))PlaySound(fxWav1);
    if (IsKeyPressed(KEY_LEFT))PlaySound(fxWav1);
    if (IsKeyPressed(KEY_DOWN))PlaySound(fxWav2);
    if (IsKeyPressed(KEY_UP))PlaySound(fxWav2);
    if (IsKeyPressed(KEY_SPACE))PlaySound(fxWav3); */

	camera.target = { 0, 0 };
	camera.offset = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
	camera.rotation = 0;
	camera.zoom = 2;
}

Game::~Game()
{
    /*UnloadSound(fxWav1);
    UnloadSound(fxWav2);
    UnloadSound(fxWav3);
    UnloadSound(fxWav4);
    UnloadSound(fxWav5);
    UnloadSound(fxWav6);
    UnloadSound(fxWav19);*/
	CloseWindow();
}

void Game::Run()
{
    Player player;
    Map map;



    enum GameState { SPLASH, TITLE, GAMEPLAY, WIN, DEATH};
    GameState currentState = SPLASH;
    Texture2D titleScreen = LoadTexture("resources/bombermanSprites/UI/NES - Bomberman - Title Screen & Text.png");
    Texture2D WinScreen = LoadTexture("resources/bombermanSprites/UI/NES- Bomberman - Win screen.png");
    Texture2D bomberman = LoadTexture("resources/bombermanSprites/General Sprites/Player_Sprites.png");

        while (!WindowShouldClose())
        {
            UpdateMusicStream(music);
            BeginDrawing();
            ClearBackground(BLACK);

            switch (currentState)
            {
            case SPLASH:
            {
                DrawText("This is a recreation of Bomberman NES. Press ENTER to continue.", 100, 350, 40, WHITE);
                DrawText("Proyecto I, Disseny i desenvolupament de videojocs, CITM Terrassa.", 200, 450, 30, WHITE);
                DrawText("Pol Cuenca, Andrea Velez, Daniel Castillero. Tutor: Alejandro Paris Gomez ", 200, 500, 30, WHITE);
                if (IsKeyPressed(KEY_ENTER)) currentState = TITLE;
                break;
            }
               
            case TITLE:
            {
                DrawTextureEx(titleScreen, { 350, 0 }, 0, 3.5, WHITE);
                DrawText("Presiona ENTER para comenzar", 500, 850, 25, WHITE);
                if (IsKeyPressed(KEY_ENTER)) currentState = GAMEPLAY;
                break;
            }
              

            case GAMEPLAY:
            {
               
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
                player.DrawPlayer(bomberman);

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
                            currentState = DEATH;
                            // CloseWindow();
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

                if (IsKeyReleased(KEY_C))
                {
                    map.ClearMap();
                }

                if (player.GetCurrentHp() <= 0)
                {
                    // CloseWindow();
                    currentState = DEATH;
                }
                if (player.victoryStatus())
                {
                    currentState = WIN;
                }
                EndMode2D();
                break;
            }
               

            case WIN:
            {
                DrawTextureEx(WinScreen, { 300, 0 }, 0, 1.5, WHITE);
                if (IsKeyPressed(KEY_ENTER)) CloseWindow();
                break;
            }

            case DEATH:
            {
                DrawText("You Lost", 700, 425, 25, WHITE);
                if (IsKeyPressed(KEY_ENTER)) CloseWindow();
                break;
            }
              
            }

            EndDrawing();
        }   

        UnloadTexture(titleScreen);
        UnloadTexture(bomberman);
        UnloadMusicStream(music);
        CloseAudioDevice();
        CloseWindow();
    
}