#include "Player.h"

#include "raylib.h"

Player::Player()
{
    rect = { 45,45,32,32 };
    //pos = { 45, 45 };		
}

void Player::UpdatePlayer(Map& map)
{
    Rectangle oldRect = rect;
    bool isMoving = false;
    float deltaTime = GetFrameTime();

    int wasLooking = mirando;

    if (IsKeyDown(KEY_RIGHT))
    {
        rect.x += playerSpeed;
        mirando = 1; // Derecha
        isMoving = true;
    }
    if (IsKeyDown(KEY_LEFT))
    {
        rect.x -= playerSpeed;
        mirando = 2; // Izquierda
        isMoving = true;
    }
    if (IsKeyDown(KEY_DOWN))
    {
        rect.y += playerSpeed;
        mirando = 0; // Abajo
        isMoving = true;
    }
    if (IsKeyDown(KEY_UP))
    {
        rect.y -= playerSpeed;
        mirando = 3; // Arriba
        isMoving = true;
    }

    if (map.CheckCollisions(rect))
    {
        rect = oldRect;
        
        isMoving = false;
    }

    const int walk_lft_rght[] = { 2, 18, 34 };  // Coordenadas sprites para cuando se mueve a Izquierda/Derecha
    const int walk_up_dwn[] = { 50, 66, 82 }; // Coordenadas sprites para cuando se mueve a Arriba/Abajo
    const int numFrames = 3;

    if (isMoving)
    {
        timerAnimacion += deltaTime;
        if (timerAnimacion >= frameSpeed)
        {
            timerAnimacion = 0.0f;
            currentFrame = (currentFrame + 1) % numFrames;
        }
    }
    else
    {
        currentFrame = 1;
        timerAnimacion = 0.0f;
    }

    if (mirando != wasLooking && isMoving) //   Reset del frame a idle para que la animacion no haga cosas raras al cambiar de direccion
    {
        currentFrame = 1;
        timerAnimacion = 0.0f;
    }

    switch (mirando)
    {
    case 0: // Abajo
        sprite_status.y = 0;
        sprite_status.x = walk_up_dwn[currentFrame];
        break;
    case 1: // Derecha
        sprite_status.y = 16;
        sprite_status.x = walk_lft_rght[currentFrame];
        break;
    case 2: // Izquierda
        sprite_status.y = 0;
        sprite_status.x = walk_lft_rght[currentFrame];
        break;
    case 3: // Arriba
        sprite_status.y = 16;
        sprite_status.x = walk_up_dwn[currentFrame];
        break;
    }

    if (IsKeyPressed(KEY_SPACE) && bombs.size() < maxBombs)
    {
        int gridX = (int)((rect.x + rect.width / 2) / 40.0f);
        int gridY = (int)((rect.y + rect.height / 2) / 40.0f);
        Vector2 bombPos = { (float)gridX * 40.0f, (float)gridY * 40.0f };

        bombs.push_back(Bomb(bombPos));
    }

    for (int i = 0; i < bombs.size();)
    {
        if (bombs[i].UpdateState(deltaTime, map))
        {
            bombs.erase(bombs.begin() + i);
        }
        else
        {
            i++;
        }
    }

    if (map.IsDoorSpawned() && CheckCollisionRecs(rect, { map.GetDoorPos().x, map.GetDoorPos().y, 40, 40 }))
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            hasWin = true;
        }
    }
}



void Player::DrawPlayer(Texture2D bomberman, Texture2D bomba)
{
    for (const Bomb& b : bombs) b.DrawBomb(bomba);
    Rectangle bombermanRecorte = { sprite_status.x,sprite_status.y,12,16 };
    Rectangle bombermanPosYtamaño = { rect.x, rect.y, 16*2, 16*2 };
    DrawTexturePro(bomberman, bombermanRecorte, bombermanPosYtamaño, {0,0}, 0, WHITE);

    DrawRectangleLines(rect.x, rect.y, rect.width, rect.height, BLACK); //  Debug para ver los colliders del jugador
}

Vector2 Player::GetPlayerPos()
{
    Vector2 position = { rect.x, rect.y };
    return position;
}

void Player::SetPlayerPos()
{
    // Setear pos en x por valor dado
    // Setear pos en y por valor dado

    rect.x = 45;
    rect.y = 45;

    wasHit = false;
}

Rectangle Player::GetPlayerRect()
{
    return rect;
}

void Player::TakeDamage()
{
    if (!wasHit)
    {
        wasHit = true;
        playerHp--;
    }
}

int Player::GetCurrentHp() const
{
    return playerHp;
}

bool Player::victoryStatus()
{
    return hasWin;
}