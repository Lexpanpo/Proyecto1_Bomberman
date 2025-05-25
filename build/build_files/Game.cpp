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



    //if (IsKeyPressed(KEY_RIGHT))PlaySound(soundArray[0]);
    //if (IsKeyPressed(KEY_LEFT))PlaySound(soundArray[0]);
    //if (IsKeyPressed(KEY_DOWN))PlaySound(soundArray[1]);
    //if (IsKeyPressed(KEY_UP))PlaySound(soundArray[1]);
    //if (IsKeyPressed(KEY_SPACE))PlaySound(soundArray[2]);


    camera.target = { 0, 0 };
    camera.offset = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
    camera.rotation = 0;
    camera.zoom = 1.5;
}

Game::~Game()
{
    CloseWindow();
}


void SpawnEnemiesForLevel(int level, Map& map, vector<Enemy>& enemies) // Función para generar enemigos según el nivel 
{
    enemies.clear();
    int enemiesToSpawn = 3 + (level - 1) * 2; // 3 al principio y +2 cada nivel

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

            if (!map.IsTileEmpty(gridX, gridY))
            {
                validSpawn = false; continue;
            }
            if (gridX <= 3 && gridY <= 3)
            {
                validSpawn = false; continue;
            }
            for (const Enemy& existingEnemy : enemies) // Bucle para comprobar si ya hay un enemigo en esa posicion
            {
                Rectangle r = existingEnemy.GetRect();
                if ((int)(r.x / 40.0f) == gridX && (int)(r.y / 40.0f) == gridY)
                {
                    validSpawn = false; break;
                }
            }
        } while (!validSpawn && attempts < 1000);   // Limite de intentos (Lo he puesto en 1000 pero se puede bajar)

        EnemyType type = (GetRandomValue(1, 10) <= 7) ? EnemyType::BALLOM : EnemyType::DORIA;

        if (validSpawn)
        {
            enemies.push_back(Enemy({ (float)gridX * 40.0f, (float)gridY * 40.0f }, type));
        }
        else
        {
            cout << "NO SE HA ENCONTRADO ESPACIO PARA EL ENEMIGO! " << i + 1 << endl;
        }
    }
}


void LoadLevel(int level, Player& player, Map& map, vector<Enemy>& enemies, bool& gameplayInitialized) // Función para cargar/reiniciar un nivel
{
    cout << "CARGANDO NIVEL " << level << endl;
    player.SetPlayerPos();
    player.bombs.clear();
    map.ResetLevel();
    SpawnEnemiesForLevel(level, map, enemies);
    explosions.clear();
    gameplayInitialized = true;
    player.wasHit = false;
}

void Game::Run()
{
    Player player;
    Map map;

    vector<Enemy> enemies;
    bool isPlaying = false;

    int currentLevel = 1;

    enum GameState { SPLASH, TITLE, GAMEPLAY, WIN, DEATH };
    GameState currentState = SPLASH;

    Texture2D titleScreen = LoadTexture("resources/bombermanSprites/UI/NES - Bomberman - Title Screen & Text.png");
    Texture2D WinScreen = LoadTexture("resources/bombermanSprites/UI/NES- Bomberman - Win screen.png");
    Texture2D bomberman = LoadTexture("resources/bombermanSprites/General Sprites/Player_Sprites.png");
    Texture2D bomba = LoadTexture("resources/bombermanSprites/General Sprites/Bomb_Sprites.png");
    Texture2D explosiones = LoadTexture("resources/bombermanSprites/General Sprites/Explosion_Sprites_v2.png");
    Texture2D walls = LoadTexture("resources/bombermanSprites/General Sprites/Walls_Sprites.png");
    Texture2D explosion = LoadTexture("resources/bombermanSprites/General Sprites/Explosion_Sprites.png");
    Texture2D enemy1 = LoadTexture("resources/bombermanSprites/General Sprites/Enemies_Sprites.png");

    static Sound soundArray[7];

    soundArray[0] = LoadSound("resources/bombermanAudio/SFX/Bomberman SFX (1).wav");
    soundArray[1] = LoadSound("resources/bombermanAudio/SFX/Bomberman SFX (2).wav");
    soundArray[2] = LoadSound("resources/bombermanAudio/SFX/Bomberman SFX (3).wav");
    soundArray[3] = LoadSound("resources/bombermanAudio/SFX/Bomberman SFX (4).wav");
    soundArray[4] = LoadSound("resources/bombermanAudio/SFX/Bomberman SFX (5).wav");
    soundArray[5] = LoadSound("resources/bombermanAudio/SFX/Bomberman SFX (6).wav");
    soundArray[6] = LoadSound("resources/bombermanAudio/SFX/Bomberman II SFX (19).wav");

    for (int i = 0; i < 6; i++) {
        SetSoundVolume(soundArray[i], 0.2f);
    }

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
                LoadLevel(currentLevel, player, map, enemies, isPlaying);
            }

            player.UpdatePlayer(map, soundArray);
            map.Update(deltaTime);

            for (Enemy& enemy : enemies)
            {
                if (enemy.IsAlive())
                {
                    enemy.Update(map, deltaTime, player.bombs);
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
                        score += enemy.GetScoreValue();
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

            if (enemies.empty() && !map.IsDoorSpawned())
            {
                map.SpawnDoor();
            }
            if (map.IsDoorSpawned())
            {
                Rectangle doorRect = { map.GetDoorPos().x, map.GetDoorPos().y, 40, 40 };

                if (CheckCollisionRecs(player.GetPlayerRect(), doorRect) && IsKeyPressed(KEY_ENTER)) {
                    currentLevel++;
                    if (currentLevel > 4)
                    {
                        currentState = WIN;
                    }
                    else
                    {
                        isPlaying = false;
                    }
                    goto gameplay_end;
                }
            }

            if (player.GetCurrentHp() <= 0)
            {
                currentState = DEATH;
                isPlaying = false;
                //enemies.clear();
            }
            else if (playerWasHit)
            {
                player.SetPlayerPos();
                explosions.clear();
                player.bombs.clear();
                player.wasHit = false;
                //enemies.clear();
                //isPlaying = false;
            }

            // Teclas de Debug
            if (IsKeyReleased(KEY_P)) // P: Hacer Daño al jugador
            {
                player.SetPlayerPos();
                player.TakeDamage();
            }
            if (IsKeyReleased(KEY_C)) // C: Eliminar todos los SoftBlocks
            {
                map.ClearMap();
            }

            // Condición de Victoria
           /* if (player.victoryStatus())
            {
                currentState = WIN;
            }*/

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

        gameplay_end:;
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
            ClearBackground(GRAY);

            BeginMode2D(camera);
            map.DrawMap(walls);
            player.DrawPlayer(bomberman, bomba);
            // Dibujar Enemigos
            for (const Enemy& enemy : enemies)
            {
                enemy.DrawEnemy(enemy1);
            }
            // Dibujar Explosiones
            for (const Explosion& e : explosions)
            {
                e.DrawExplosion(explosiones);
            }
            EndMode2D();

            DrawText(TextFormat("Level: %d", currentLevel), 20, 20, 30, WHITE);

            DrawText(TextFormat("Score: %06d", score), GetScreenWidth() / 2 - 100, 20, 30, WHITE);

            DrawText(TextFormat("Lives: %d", player.GetCurrentHp()), GetScreenWidth() - 150, 20, 30, WHITE);

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
    UnloadTexture(explosiones);
    UnloadTexture(walls);
    UnloadTexture(explosion);
    UnloadTexture(enemy1);
    UnloadMusicStream(music);
    for (int i = 0; i < 6; i++) UnloadSound(soundArray[i]);

    CloseAudioDevice();
    CloseWindow();
}