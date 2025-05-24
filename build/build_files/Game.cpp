#include "Game.h"
#include "Player.h"
#include "Map.h"
#include "Bomb.h"
#include "Explosion.h"
#include "enemy.h"

#include <iostream>
#include <vector>
#include <algorithm>
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

    vector<Enemy> enemies;
    bool isPlaying = false;

    enum GameState { SPLASH, TITLE, GAMEPLAY, WIN, DEATH};
    GameState currentState = SPLASH;

    Texture2D titleScreen = LoadTexture("resources/bombermanSprites/UI/NES - Bomberman - Title Screen & Text.png");
    Texture2D WinScreen = LoadTexture("resources/bombermanSprites/UI/NES- Bomberman - Win screen.png");
    Texture2D bomberman = LoadTexture("resources/bombermanSprites/General Sprites/Player_Sprites.png");
    Texture2D bomba = LoadTexture("resources/bombermanSprites/General Sprites/Bomb_Sprites.png");
    Texture2D walls = LoadTexture("resources/bombermanSprites/General Sprites/Walls_Sprites.png");

    while (!WindowShouldClose())
    {
        UpdateMusicStream(music);
        float deltaTime = GetFrameTime();

        switch (currentState)
        {
        case SPLASH:
        {
            if (IsKeyPressed(KEY_ENTER)) currentState = TITLE;
            break;
        }
        case TITLE:
        {
            if (IsKeyPressed(KEY_ENTER)) currentState = GAMEPLAY;
            break;
        }
        case GAMEPLAY:
        {
            if (!isPlaying)
            {
                int enemiesToSpawn = 5; // Numero de enmeigos que se spawnean

                for (int i = 0; i < enemiesToSpawn; ++i)
                {
                    int gridX, gridY;
                    bool validSpawn;
                    int attempts = 0; // Variable para qque el bucle no sea infinito

                    do {
                        validSpawn = true;
                        attempts++;

                        gridX = GetRandomValue(1, 29);
                        gridY = GetRandomValue(1, 11);

                        if (!map.IsTileEmpty(gridX, gridY)) {
                            validSpawn = false;
                            continue;
                        }

                        if (gridX <= 3 && gridY <= 3) {
                            validSpawn = false;
                            continue;
                        }

                        for (const Enemy& existingEnemy : enemies) //   Bucle para comprobar si ya hay un enemigo en esa posicion
                        {
                            Rectangle r = existingEnemy.GetRect();
                            int enemyGridX = (int)(r.x / 40.0f);
                            int enemyGridY = (int)(r.y / 40.0f);
                            if (gridX == enemyGridX && gridY == enemyGridY) {
                                validSpawn = false;
                                break;
                            }
                        }

                        
                    } while (!validSpawn && attempts < 1000); //    Limite de intentos (Lo he puesto en 1000 pero se puede bajar)

                    if (validSpawn) {
                        enemies.push_back(Enemy({ (float)gridX * 40.0f, (float)gridY * 40.0f }));
                    }
                    else {
                        cout << "NO SE HA ENCONTRADO ESPACIO PARA EL ENEMIGO! " << i + 1 << endl;
                    }
                }

                isPlaying = true;
            }

            player.UpdatePlayer(map);

            for (Enemy& enemy : enemies)
            {
                if (enemy.IsAlive())
                {
                    enemy.Update(map, deltaTime);
                }
            }

            for (int i = 0; i < explosions.size();)
            {
                if (explosions[i].UpdateStatus(deltaTime))
                {
                    explosions.erase(explosions.begin() + i);
                }
                else
                {
                    i++;
                }
            }

            bool playerWasHit = false;

            
            for (const Explosion& e : explosions)   // Colisiones con las eplosiones
            {
                if (!playerWasHit && CheckCollisionRecs(player.GetPlayerRect(), e.GetExplosionRect()))
                {
                    player.TakeDamage();
                    playerWasHit = true;
                }
                for (Enemy& enemy : enemies)
                {
                    if (enemy.IsAlive() && CheckCollisionRecs(e.GetExplosionRect(), enemy.GetRect()))
                    {
                        enemy.Kill();
                    }
                }
            }

            
            for (const Enemy& enemy : enemies)  //Colisiones del jugador y los enemigos
            {
                if (!playerWasHit && enemy.IsAlive() && CheckCollisionRecs(player.GetPlayerRect(), enemy.GetRect()))
                {
                    player.TakeDamage();
                    playerWasHit = true;
                    break;
                }
            }

            enemies.erase(remove_if(enemies.begin(), enemies.end(),
                [](const Enemy& e) { return !e.IsAlive(); }),
                enemies.end());

            if (player.GetCurrentHp() <= 0)
            {
                currentState = DEATH;
                enemies.clear();
                isPlaying = false;
            }
            else if (playerWasHit)
            {
                player.SetPlayerPos();
                explosions.clear();
                enemies.clear();
                isPlaying = false;
            }

            // Teclas de Debug
            if (IsKeyReleased(KEY_P)) //    Hacer Daño al jugador
            {
                player.SetPlayerPos();
                player.TakeDamage();
            }
            if (IsKeyReleased(KEY_C)) //    Eliminar todos los SoftBlocks
            {
                map.ClearMap();
            }

            // Condición de Victoria
            if (player.victoryStatus())
            {
                currentState = WIN;
            }

            // Actualizar Cámara
            Vector2 targetPos = player.GetPlayerPos();
            camera.target = targetPos;
            float mapWidth = 31 * 40;
            float mapHeight = 13 * 40;
            float halfWidth = GetScreenWidth() / (2.0f * camera.zoom);
            float halfHeight = GetScreenHeight() / (2.0f * camera.zoom);
            if (camera.target.x < halfWidth) camera.target.x = halfWidth;
            if (camera.target.y < halfHeight) camera.target.y = halfHeight;
            if (camera.target.x > mapWidth - halfWidth) camera.target.x = mapWidth - halfWidth;
            if (camera.target.y > mapHeight - halfHeight) camera.target.y = mapHeight - halfHeight;

            break;
        }
        case WIN:
            if (IsKeyPressed(KEY_ENTER)) CloseWindow();
            break;
        case DEATH:
            if (IsKeyPressed(KEY_ENTER)) CloseWindow();
            break;
        }

        // --- Fase de Dibujado ---
        BeginDrawing();
        ClearBackground(BLACK);

        switch (currentState)
        {
        case SPLASH:
        {
            DrawText("This is a recreation of Bomberman NES. Press ENTER to continue.", 100, 350, 40, WHITE);
            DrawText("Proyecto I, Disseny i desenvolupament de videojocs, CITM Terrassa.", 200, 450, 30, WHITE);
            DrawText("Pol Cuenca, Andrea Velez, Daniel Castillero. Tutor: Alejandro Paris Gomez ", 200, 500, 30, WHITE);
            break;
        }
        case TITLE:
        {
            DrawTextureEx(titleScreen, { 350, 0 }, 0, 3.5, WHITE);
            DrawText("Presiona ENTER para comenzar", 500, 850, 25, WHITE);
            break;
        }
        case GAMEPLAY:
        {
            BeginMode2D(camera);
            map.DrawMap(walls);
            player.DrawPlayer(bomberman, bomba);
            // Dibujar Enemigos
            for (const Enemy& enemy : enemies)
            {
                enemy.Draw();
            }
            // Dibujar Explosiones
            for (const Explosion& e : explosions)
            {
                e.DrawExplosion();
            }
            EndMode2D();

            break;
        }
        case WIN:
        {
            DrawTextureEx(WinScreen, { 300, 0 }, 0, 1.5, WHITE);
            DrawText("Presiona ENTER para salir", 650, 700, 25, WHITE);
            break;
        }
        case DEATH:
        {
            DrawText("You Lost", 700, 425, 50, RED);
            DrawText("Presiona ENTER para salir", 650, 500, 25, WHITE);
            break;
        }
        }

        EndDrawing();
    }

    UnloadTexture(titleScreen);
    UnloadTexture(WinScreen);
    UnloadTexture(bomberman);
    UnloadTexture(bomba);
    UnloadTexture(walls);
    UnloadMusicStream(music);
    CloseAudioDevice();
    CloseWindow();    
}