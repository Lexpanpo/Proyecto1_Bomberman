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
    //Vector2 oldPos = pos;

    if (IsKeyDown(KEY_RIGHT))
    {
        rect.x += playerSpeed;
        mirando = 1;

        sprite_status.y = 16;
        if (sprite_status.x >= 34)
        {
            sprite_status.x = 2;
        }
        else
        {
            sprite_status.x += 16;
        }
    }
    if (IsKeyDown(KEY_LEFT))
    {
        rect.x -= playerSpeed;
        mirando = 2;

        sprite_status.y = 0;
        if (sprite_status.x >= 34)
        {
            sprite_status.x = 2;
        }
        else
        {
            sprite_status.x += 16;
        }
    }
    if (IsKeyDown(KEY_DOWN))
    {
        rect.y += playerSpeed;
        mirando = 0;

        sprite_status.y = 0;
        if (sprite_status.x >= 82)
        {
            sprite_status.x = 50;
        }
        else
        {
            sprite_status.x += 16;
        }
    }
    if (IsKeyDown(KEY_UP))
    {
        rect.y -= playerSpeed;
        mirando = 3;
        sprite_status.y = 16;
        if (sprite_status.x >= 82)
        {
            sprite_status.x = 50;
        }
        else
        {
            sprite_status.x += 16;
        }
    }      

    int posX = (pos.x + 16) / 40;
    int posY = (pos.y + 16) / 40;

    if (map.CheckCollisions(rect))
    {
        rect = oldRect;
        //pos = oldPos;
    }

    if (IsKeyPressed(KEY_SPACE))
    {
        Vector2 bombPos = { (rect.x / 40) * 40, (rect.y / 40) * 40 };
        //Bomb newBomb(bombPos);

        bombs.push_back(Bomb(bombPos));
        //newBomb.DrawBomb();
    }

    for (int i = 0; i < bombs.size();)
    {
        if (bombs[i].UpdateState(GetFrameTime(), map))
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
            
            // CloseWindow();

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