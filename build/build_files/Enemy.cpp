#include "Enemy.h"
#include "Bomb.h"

#include <vector>
#include <cstdlib>

Enemy::Enemy(Vector2 startPos, EnemyType enemyType)
{
	alive = true;

    state = ALIVE;

	pos = startPos;
	rect = { pos.x + 4, pos.y + 4, 35, 35 };

	//moveTimer = 0.0f;

    type = enemyType;

    currentFrame = 0;
    animationTimer = 0.0f;

    spriteStatus = { 0.0f, 0.0f, 16.0f, 16.0f };

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
    if (state == DEAD) return;

	if (state == ALIVE)
	{
        Vector2 oldPos = pos;
		Move(map, deltaTime, playerBombs);
        bool moved = (pos.x != oldPos.x || pos.y != oldPos.y);

        if (moved)
        {
            animationTimer += deltaTime;
            if (animationTimer >= framespeed)
            {
                animationTimer = 0.0f;
                currentFrame = (currentFrame + 1) % 3;
            }
        }
        else
        {
            currentFrame = 1;
            animationTimer = 0.0f;
        }

        if (this->direction == 1)
        {
            facingRight = true;
        }
        else if (this->direction == 2)
        {
            facingRight = false;

        }

        if (!facingRight)
        {
            spriteStatus.x = (currentFrame + 3) * 16;
        }
        else
        {
            spriteStatus.x = currentFrame * 16;
        }

        spriteStatus.y = 0.0f;
	}
    else if (state == DYING_1)
    {
        spriteStatus.x = 6 * 16;
        spriteStatus.y = 0.0f;

        deathTimer += deltaTime;;

        if (deathTimer >= 1.0f)
        {
            state = DYING_2;
            deathTimer = 0.0f;
            currentFrame = 7;
        }
    }
    else if (state == DYING_2)
    {
        deathTimer += deltaTime;

        if (deathTimer >= deathFrameSpeed)
        {
            deathTimer = 0.0f;
            if (currentFrame < 10)
            {
                currentFrame++;
            }
            else
            {
                state = DEAD;
                this->alive = false;
            }
        }

        if (state == DYING_2)
        {
            spriteStatus.x = (float)currentFrame * 16.0f;
            spriteStatus.y = 0.0f;
        }
    }
}

void Enemy::Draw(Texture2D enemyTexture) const
{
	if (state != DEAD)
	{
        //Rectangle destRect = { pos.x, pos.y, 40, 40 };
        DrawTexturePro(enemyTexture, spriteStatus, this->rect, { 0,0 }, 0, WHITE);
		//DrawRectangleRec(rect, PURPLE);
        DrawRectangleLines(rect.x, rect.y, rect.width, rect.height, RED); //  Debug para ver los colliders del jugador

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

EnemyState Enemy::GetState() const
{
    return state;
}

bool Enemy::IsAlive() const
{
	return state != DEAD;
}

void Enemy::Kill()
{
	//alive = false;

    if (state == ALIVE)
    {
        state = DYING_1;
        deathTimer = 0.0f;
        currentFrame = 6;
    }
}