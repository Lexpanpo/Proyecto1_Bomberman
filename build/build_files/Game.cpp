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

void LoadLevel(int level, Player& player, Map& map, vector<Enemy>& enemies, bool& gameplayInitialized, float& timer, bool& pointsFlag) // Función para cargar/reiniciar un nivel
{
    cout << "CARGANDO NIVEL " << level << endl;
    player.SetPlayerPos();
    player.bombs.clear();
    map.ResetLevel();
    SpawnEnemiesForLevel(level, map, enemies);
    explosions.clear();
    gameplayInitialized = true;
    timer = 200.0f;
    pointsFlag = true;
}

void Game::Run()
{
    Player player;
    Map map;

    vector<Enemy> enemies;
    bool isPlaying = false;

    int currentLevel = 1;

    float levelTimer = 200.0f;
    bool canEarnPoints = true;

    enum GameState { SPLASH, TITLE, LEVEL_START,GAMEPLAY, DEATH_PAUSE, WIN, DEATH };
    float levelStartCounter = 0.0f;
    float deathPauseCounter = 0.0f;
    GameState currentState = SPLASH;
    GameState previousState = SPLASH;
    Music* currentMusicStream = nullptr;

    Texture2D titleScreen = LoadTexture("resources/bombermanSprites/UI/NES - Bomberman - Title Screen & Text.png");
    Texture2D WinScreen = LoadTexture("resources/bombermanSprites/UI/NES- Bomberman - Win screen.png");
    Texture2D bomberman = LoadTexture("resources/bombermanSprites/General Sprites/Player_Sprites.png");
    Texture2D bomba = LoadTexture("resources/bombermanSprites/General Sprites/Bomb_Sprites.png");
    Texture2D explosiones = LoadTexture("resources/bombermanSprites/General Sprites/Explosion_Sprites_v2.png");
    Texture2D walls = LoadTexture("resources/bombermanSprites/General Sprites/Walls_Sprites.png");
    Texture2D powerUps = LoadTexture("resources/bombermanSprites/General Sprites/PowerUps_Sprites_v2.png");

    Font customFont = LoadFont("resources/nesFont/smb_font.ttf");

    InitAudioDevice();
    static Sound soundArray[7];

    soundArray[0] = LoadSound("resources/bombermanAudio/SFX/Bomberman SFX (1).wav"); //walk
    soundArray[1] = LoadSound("resources/bombermanAudio/SFX/Bomberman SFX (2).wav"); //walk
    soundArray[2] = LoadSound("resources/bombermanAudio/SFX/Bomberman SFX (3).wav"); //place bomb
    soundArray[3] = LoadSound("resources/bombermanAudio/SFX/Bomberman SFX (4).wav"); //poweup
    soundArray[4] = LoadSound("resources/bombermanAudio/SFX/Bomberman SFX (5).wav");//player dies
    soundArray[5] = LoadSound("resources/bombermanAudio/SFX/Bomberman SFX (6).wav"); //defeat enemies
    soundArray[6] = LoadSound("resources/bombermanAudio/SFX/Bomberman II SFX (19).wav"); //explosion

    static Music musicArray[9];

    musicArray[0] = LoadMusicStream("resources/bombermanAudio/music/StageTheme.wav"); //stage
    musicArray[1] = LoadMusicStream("resources/bombermanAudio/music/Bomberman (NES) Music - Level Complete.wav"); //level complete
    musicArray[2] = LoadMusicStream("resources/bombermanAudio/music/Bomberman (NES) Music - Level Start.wav"); //level start
    musicArray[3] = LoadMusicStream("resources/bombermanAudio/music/Bomberman-_NES_-Music-Find-The-Door.wav"); //find the door
    musicArray[4] = LoadMusicStream("resources/bombermanAudio/music/Bomberman-_NES_-Music-Game-Over.wav"); //game over
    musicArray[5] = LoadMusicStream("resources/bombermanAudio/music/Bomberman-_NES_-Music-Title-Screen-_dE0azz5nJeg_.wav"); //titlescreen
    musicArray[6] = LoadMusicStream("resources/bombermanAudio/music/Bomberman (NES) Music - Just Died.wav"); //just died
    musicArray[7] = LoadMusicStream("resources/bombermanAudio/music/Bomberman-_NES_-Music-Invincibility-Theme.wav"); //invisivility
    musicArray[8] = LoadMusicStream("resources/bombermanAudio/music/Bomberman-_NES_-Music-Ending-Theme-_m7DR-tiNWC8_.wav"); //win ending

    //musicArray[0].looping = true;  // Stage Theme SÍ se repite
    //musicArray[1].looping = false; // Level Complete NO se repite
    //musicArray[2].looping = false; // Level Start NO se repite
    //musicArray[3].looping = true;  // Find Door SÍ se repite
    //musicArray[4].looping = false; // Game Over NO se repite
    //musicArray[5].looping = true;  // Title Screen SÍ se repite
    //musicArray[6].looping = false; // Just Died NO se repite
    //musicArray[7].looping = true;  // Invincibility SÍ se repite
    //musicArray[8].looping = false; // Ending NO se repite

 
    for (int i = 0; i < 6; i++) {
        SetSoundVolume(soundArray[i], 0.2f);
    } 

    for (int i = 0; i < 6; i++) {
        SetMusicVolume(musicArray[i], 0.2f);
    }

    Texture2D ballomSprites = LoadTexture("resources/bombermanSprites/General Sprites/Ballom_Sprites.png");
    Texture2D doriaSprites = LoadTexture("resources/bombermanSprites/General Sprites/Doria_Sprites.png");


    while (!WindowShouldClose())
    {
        /*UpdateMusicStream(musicArray[0]);
        UpdateMusicStream(musicArray[1]);
        UpdateMusicStream(musicArray[2]);
        UpdateMusicStream(musicArray[3]);
        UpdateMusicStream(musicArray[4]);
        UpdateMusicStream(musicArray[5]);
        UpdateMusicStream(musicArray[6]);
        UpdateMusicStream(musicArray[7]);
        UpdateMusicStream(musicArray[8]);*/

        float deltaTime = GetFrameTime();

        if (currentState != previousState)
        {
            if (currentMusicStream != nullptr)
            {
                StopMusicStream(*currentMusicStream);
            }

            switch (currentState)
            {
            case SPLASH:
                currentMusicStream = nullptr;
                break;
            case TITLE:
                currentMusicStream = &musicArray[5];
                break;
            case LEVEL_START:
                currentMusicStream = &musicArray[2];
                break;
            case GAMEPLAY:
                currentMusicStream = &musicArray[0];
                break;
            case DEATH_PAUSE:
                currentMusicStream = &musicArray[6];
                break;
            case WIN:
                currentMusicStream = &musicArray[8];
                break;
            case DEATH:
                currentMusicStream = &musicArray[4];
                break;
            default:
                currentMusicStream = nullptr;
                break;
            }

            if (currentMusicStream != nullptr)
            {
                StopMusicStream(*currentMusicStream);
                PlayMusicStream(*currentMusicStream);
            }

            previousState = currentState;
        }

        if (currentMusicStream != nullptr)
        {
            UpdateMusicStream(*currentMusicStream);
        }

        switch (currentState)
        {
        case SPLASH:
        {
            //PlayMusicStream(musicArray[5]);
            if (IsKeyPressed(KEY_ENTER)) currentState = TITLE;
            break;
        }
        case TITLE:
        {
            currentLevel = 1;
            //musicArray[2].looping = false;
            //PlayMusicStream(musicArray[2]);
            if (IsKeyPressed(KEY_ENTER)) currentState = GAMEPLAY;
            if (IsKeyPressed(KEY_ENTER))
            {
                currentState = LEVEL_START;
                levelStartCounter = 2.0f;
                isPlaying = false;
            }
            break;
        }
        case LEVEL_START:
        {
            //UpdateMusicStream(music);
            //PlayMusicStream(musicArray[0]);
            levelStartCounter -= deltaTime;

            if (levelStartCounter <= 0)
            {
                currentState = GAMEPLAY;
                isPlaying = false;
            }
            
            break;
        }
        case GAMEPLAY:
        {
            PauseMusicStream(musicArray[5]);
            if (!isPlaying)
            {
                LoadLevel(currentLevel, player, map, enemies, isPlaying, levelTimer, canEarnPoints);
            }

            if (levelTimer > 0 && player.GetState() == P_ALIVE)
            {
                levelTimer -= deltaTime;

                if (levelTimer <= 0)
                {
                    levelTimer = 0;
                    if (canEarnPoints)
                    {
                        canEarnPoints = false;
                    }
                }
            }

            player.UpdatePlayer(map, deltaTime,soundArray, musicArray);
            bool playerHitByExplosion = false;
            bool playerHitByEnemy = false;

            map.Update(deltaTime);

            vector<PowerUp>& powerUps = map.GetPowerUps();

            for (int i = powerUps.size() - 1; i >= 0; i--)
            {
                if (powerUps[i].active && CheckCollisionRecs(player.GetPlayerRect(), powerUps[i].rect)) 
                {
                    player.PickPowerUp(powerUps[i].type);
                    //Sonido de cuando pillas un powerup
                    map.RemovePowerUpAt(i);
                }
            }

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
                if (!player.HasFlamePass() && player.GetState() == P_ALIVE && !playerWasHit && CheckCollisionRecs(player.GetPlayerRect(), e.GetExplosionRect()))
                {
                    
                    player.TakeDamage();
                    playerHitByExplosion = true;
                    playerWasHit = true;
                }
                for (Enemy& enemy : enemies)
                {
                    if (enemy.GetState() == EnemyState::ALIVE && CheckCollisionRecs(e.GetExplosionRect(), enemy.GetRect()))
                    {
                        if (canEarnPoints)
                        {
                            score += enemy.GetScoreValue();
                        }
                        enemy.Kill();
                    }
                }
            }
            
            if (player.GetState() == P_ALIVE && !playerWasHit) //Colisiones del jugador y los enemigos
            {
                for (const Enemy& enemy : enemies)  
                {
                    if (enemy.GetState() == EnemyState::ALIVE && CheckCollisionRecs(player.GetPlayerRect(), enemy.GetRect()))
                    {
                    
                        player.TakeDamage();
                        playerHitByEnemy = true;
                        playerWasHit = true;
                        break;
                    }
                }
            }

            if (player.GetState() == P_DYING && player.DeathAnimationFinished())
            {
                currentState = DEATH_PAUSE;
                deathPauseCounter = 1.0f;                      
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

                    //musicArray[1].looping = false;
                    //PlayMusicStream(musicArray[1]);

                    if (currentLevel > 4)
                    {
                        //PlayMusicStream(musicArray[8]);
                        currentState = WIN;
                      
                    }
                    else
                    {
                        currentState = LEVEL_START;
                        levelStartCounter = 2.0f;
                        isPlaying = false;
                    }
                }
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
            if (IsKeyReleased(KEY_K)) // K: Matar a todos los enemigos
            {
                for (Enemy& enemy : enemies)
                {                   
                    enemy.Kill();
                }
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
        case DEATH_PAUSE:
        {
            deathPauseCounter -= deltaTime;

            if (deathPauseCounter <= 0)
            {
               if (player.GetCurrentHp() > 0)
               {
                   currentState = LEVEL_START;
                   levelStartCounter = 2.0f;
                   isPlaying = false;
               }
               else
               {
                   //musicArray[6].looping = false;
                   //PlayMusicStream(musicArray[6]);
                   //PlayMusicStream(musicArray[4]);
                   currentState = DEATH;
                   isPlaying = false;
               }
            }

            break;
        }
        case WIN:
            //PauseMusicStream(musicArray[0]);
            //PlayMusicStream(musicArray[1]);
            if (IsKeyPressed(KEY_ENTER)) currentState = TITLE;
            break;
        case DEATH:
            //PauseMusicStream(musicArray[0]);
            if (IsKeyPressed(KEY_ENTER)) currentState = TITLE;
            break;
        }

        // --- Fase de Dibujado ---
        BeginDrawing();
        ClearBackground(BLACK);

        switch (currentState)
        {
        case SPLASH:
        {            
            DrawTextEx(customFont, "This is a recreation of Bomberman NES. Press ENTER to continue.", { 75, 350}, 25, 1.0f, WHITE);
            DrawTextEx(customFont, "Proyecto I, Disseny i desenvolupament de videojocs, CITM Terrassa.", { 180, 450 }, 20, 1.0f, WHITE);
            DrawTextEx(customFont, "Pol Cuenca, Andrea Velez, Daniel Castillero. Tutor: Alejandro Paris Gomez ", { 125, 500 }, 20, 1.0f, WHITE);
            break;
        }
        case TITLE:
        {
            DrawTextureEx(titleScreen, { 350, 0 }, 0, 3.5, WHITE);
            DrawTextEx(customFont, "Presiona ENTER para comenzar", { 500, 850 }, 25, 1.0f, WHITE);
            break;
        }
        case LEVEL_START:
        {
            ClearBackground(BLACK);
            Vector2 textSize = MeasureTextEx(customFont, TextFormat("Level: %d", currentLevel), 60, 1.0f);
            DrawTextEx(customFont, TextFormat("Level: %d", currentLevel), { GetScreenWidth() / 2.0f - textSize.x / 2.0f, GetScreenHeight() / 2.0f - 30.0f }, 60, 1.0f, WHITE);
            break;
        }
        case GAMEPLAY:
        {
            ClearBackground(GRAY);

            BeginMode2D(camera);
            map.DrawMap(walls);
            map.DrawPowerUps(powerUps);
            player.DrawPlayer(bomberman, bomba);

            // Dibujar Enemigos
            for (const Enemy& enemy : enemies)
            {
                Texture2D currentTexture;

                switch (enemy.GetType())
                {
                case BALLOM:
                    currentTexture = ballomSprites;
                    break;

                case DORIA:
                    currentTexture = doriaSprites;
                    break;
                }

                enemy.Draw(currentTexture);
            }

            // Dibujar Explosiones
            for (const Explosion& e : explosions)
            {
                e.DrawExplosion(explosiones);
            }

            EndMode2D();

           // DrawText(TextFormat("Level: %d", currentLevel), 20, 20, 30, WHITE);
            DrawTextEx(customFont, TextFormat("TIME %03d", (int)levelTimer), { 20, 20 }, 30, 1.0f, WHITE);
            DrawTextEx(customFont, TextFormat("SCORE %06d", score), { GetScreenWidth() / 2.0f - 150, 20 }, 30, 1.0f, WHITE);
            DrawTextEx(customFont, TextFormat("LIVES %d", player.GetCurrentHp()), { (float)GetScreenWidth() - 225, 20 }, 30, 1.0f, WHITE);

            break;
        }
        case DEATH_PAUSE:
        {
            ClearBackground(GRAY);

            BeginMode2D(camera);
            map.DrawMap(walls);
            player.DrawPlayer(bomberman, bomba);

            // Dibujar Enemigos
            for (const Enemy& enemy : enemies)
            {
                Texture2D currentTexture;

                switch (enemy.GetType())
                {
                case BALLOM:
                    currentTexture = ballomSprites;
                    break;

                case DORIA:
                    currentTexture = doriaSprites;
                    break;
                }

                enemy.Draw(currentTexture);
            }

            // Dibujar Explosiones
            for (const Explosion& e : explosions)
            {
                e.DrawExplosion(explosiones);
            }

            EndMode2D();

            //DrawText(TextFormat("Level: %d", currentLevel), 20, 20, 30, WHITE);
            DrawTextEx(customFont, TextFormat("TIME %03d", (int)levelTimer), { 20, 20 }, 30, 1.0f, WHITE);
            DrawTextEx(customFont, TextFormat("SCORE %06d", score), { GetScreenWidth() / 2.0f - 150, 20 }, 30, 1.0f, WHITE);
            DrawTextEx(customFont, TextFormat("LIVES %d", player.GetCurrentHp()), { (float)GetScreenWidth() - 225, 20 }, 30, 1.0f, WHITE);

            break;
        }
        case WIN:
        {
            DrawTextureEx(WinScreen, { 250, 0 }, 0, 1.5, WHITE);
            DrawTextEx(customFont, "Presiona ENTER para volver", { 550, 700 }, 25, 1.0f, WHITE);
            break;
        }
        case DEATH:
        {
            DrawTextEx(customFont, "You Lost", { 560, 425 }, 50, 1.0f, RED);
            DrawTextEx(customFont, "Presiona ENTER para volver", { 450, 500 }, 25, 1.0f, WHITE);
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
    UnloadTexture(ballomSprites);
    UnloadTexture(doriaSprites);
    UnloadTexture(powerUps);

    UnloadFont(customFont);

    for (int i = 0; i < 9; i++) UpdateMusicStream(musicArray[i]);
    for (int i = 0; i < 7; i++) UnloadSound(soundArray[i]);

    CloseAudioDevice();
    CloseWindow();
}