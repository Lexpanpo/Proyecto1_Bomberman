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
    }
    if (IsKeyDown(KEY_LEFT))
    {
        rect.x -= playerSpeed;
    }
    if (IsKeyDown(KEY_DOWN))
    {
        rect.y += playerSpeed;
    }
    if (IsKeyDown(KEY_UP))
    {
        rect.y -= playerSpeed;
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
}

void Player::DrawPlayer()
{
    for (const Bomb& b : bombs) b.DrawBomb();

    DrawRectangleRec(rect, RED);
}

Vector2 Player::GetPlayerPos()
{
    Vector2 position = { rect.x, rect.y };
    return position;
}