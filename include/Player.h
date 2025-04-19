#pragma once

#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"

class Player
{
private:
    int playerHp;
    Vector2 position;
    float acceleration;
    Color color;

public:
    Player() {};

    Player(int playerHp, Vector2 posVector, Color color)
    {
        position = posVector;
        this->color = color;
        this->playerHp = playerHp;

    }

    Vector2 getPos()
    {
        return position;
    }

    float getXPos()
    {
        return position.x;
    }

    void updatePos(float speedX, float speedY)
    {
        position.x += speedX;
        position.y += speedY;
    }
};

#endif

