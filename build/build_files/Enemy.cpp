#include "Enemy.h"
#include <cstdlib>

Enemy::Enemy(Vector2 startPos)
{
	alive = true;

	pos = startPos;

	rect = { pos.x + 4, pos.y + 4, 32, 32 };
	speed = 1.0f;
	moveTimer = 0.0f;
	ChangeDirection();	
}

void Enemy::Update(Map& map, float deltaTime)
{
	if (alive)
	{
		Move(map, deltaTime);
	}
}

void Enemy::Draw() const
{
	if (alive)
	{
		DrawRectangleRec(rect, PURPLE);
	}
}

Rectangle Enemy::GetRect() const
{
	return rect;
}

void Enemy::ChangeDirection()
{
	direction = GetRandomValue(0, 3); // 0 --> Abajo, 1 --> Derecha, 2 --> Izquierda, 3 --> Arriba
}

void Enemy::Move(Map& map, float deltaTime)
{
    Vector2 nextPos = pos;
    float moveAmount = speed;

    switch (direction)
    {
    case 0: nextPos.y += moveAmount; break; // Abajo
    case 1: nextPos.x += moveAmount; break; // Derecha
    case 2: nextPos.x -= moveAmount; break; // Izquierda
    case 3: nextPos.y -= moveAmount; break; // Arriba
    }

    Rectangle nextRect = { nextPos.x + 4, nextPos.y + 4, rect.width, rect.height }; // Creo un rectángulo para ver si en la siguiente posición hay algun bloque

    if (map.CheckCollisions(nextRect))
    {
        ChangeDirection();
    }
    else
    {
        pos = nextPos;
        rect.x = pos.x + 4;
        rect.y = pos.y + 4;

        if (GetRandomValue(0, 100) < 5) // 5% de probabilidad
        {
            ChangeDirection();
        }
    }
}

bool Enemy::IsAlive() const
{
	return alive;
}

void Enemy::Kill()
{
	alive = false;
}