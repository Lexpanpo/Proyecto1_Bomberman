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
}

void Player::DrawPlayer()
{
    DrawRectangleRec(rect, RED);
}