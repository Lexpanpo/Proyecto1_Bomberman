#include "Enemy.h"
#include <cstdlib>

Enemy::Enemy(Vector2 startPos, EnemyType enemyType)
{
	alive = true;

	pos = startPos;

	rect = { pos.x + 4, pos.y + 4, 32, 32 };
	speed = 1.0f;
    timer = 2.0f;
	moveTimer = 0.0f;

    type = enemyType;

    switch (type)
    {
        case BALLOM:
            speed = 1.0f;
            scoreValue = 100;
            break;
        case DORIA:
            speed = 0.5f;
            scoreValue = 1000;
            break;
    }

	ChangeDirection();	
}

void Enemy::Update(Map& map, float deltaTime, const vector<Bomb>& playerBombs)
{
	if (alive)
	{
		Move(map, deltaTime, playerBombs);
	}

    const int enemy_sprites_x[] = { 0, 16, 32, 48, 64,80 };
    const int numFrames = 6;

    animationTimer += deltaTime;

    if (animationTimer >= frameSpeed)
    {
        animationTimer = 0.0f;
        currentFrame = (currentFrame + 1) % numFrames;
        sprite_status.x = (float)enemy_sprites_x[currentFrame];
    }

    timer -= deltaTime;
}

void Enemy::DrawEnemy(Texture2D enemy) const
{
	if (alive)
    {

        Rectangle enemyRecorte = { sprite_status.x,sprite_status.y,16,16 };
        Rectangle enemyPosYtamaño = { pos.x, pos.y,20 * 2 , 20 * 2 };
        DrawTexturePro(enemy, enemyRecorte, enemyPosYtamaño, { 0,0 }, 0, WHITE);

	}
}

Rectangle Enemy::GetRect() const
{
	return rect;
}

EnemyType Enemy::GetType() const
{
    return type;
}

int Enemy::GetScoreValue() const
{
    return scoreValue;
}

void Enemy::ChangeDirection()
{
	direction = GetRandomValue(0, 3); // 0 --> Abajo, 1 --> Derecha, 2 --> Izquierda, 3 --> Arriba
}

void Enemy::Move(Map& map, float deltaTime, const vector<Bomb>& playerBombs)
{
    Vector2 nextPos = pos;
    float moveAmount = speed;
    Rectangle currentRec = rect;

    switch (direction)
    {
    case 0: nextPos.y += moveAmount; break; // Abajo
    case 1: nextPos.x += moveAmount; break; // Derecha
    case 2: nextPos.x -= moveAmount; break; // Izquierda
    case 3: nextPos.y -= moveAmount; break; // Arriba
    }

    Rectangle nextRect = { nextPos.x + 4, nextPos.y + 4, rect.width, rect.height }; // Creo un rectángulo para ver si en la siguiente posición hay algun bloque

    // Colisiones con el mapa y las bombas

    bool ignoreSoftBlocks = (this->type == EnemyType::DORIA);

    bool collisionDetected = map.CheckCollisions(nextRect, ignoreSoftBlocks);

    if (!collisionDetected)
    {
        for (const Bomb& b : playerBombs)
        {
            Rectangle bombRect = b.GetRect();

            if (CheckCollisionRecs(nextRect, bombRect))
            {
                if (!CheckCollisionRecs(currentRec, bombRect))
                {
                    collisionDetected = true;
                    break;
                }
            }
        }
    }

    if (collisionDetected)
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