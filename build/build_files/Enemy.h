#pragma once

#include "Map.h"
#include "Bomb.h"

#include <vector>
#include "raylib.h"

using namespace std;

enum EnemyType {
    BALLOM,
    DORIA
};

enum EnemyState
{
    ALIVE,
    DYING_1,
    DYING_2,
    DEAD
};

class Enemy
{
public:
	Enemy(Vector2 startPos, EnemyType type);

	void Update(Map& map, float deltaTime, const vector<Bomb>& playerBombs);

	void Draw(Texture2D enemyTexture)const;

	Rectangle GetRect() const;

    EnemyType GetType() const;
    int GetScoreValue() const;

    EnemyState GetState() const;
	bool IsAlive() const;
	void Kill();

private:
    Vector2 pos;
    Rectangle rect;

    float speed;
    int direction;
    //float moveTimer;

    bool alive;
   
    void Move(Map& map, float deltaTime, const vector<Bomb>& playerBombs);
    void ChangeDirection();

    EnemyType type;
    int scoreValue;

    Rectangle spriteStatus;
    float animationTimer = 0.0f;
    int currentFrame = 0;
    float framespeed = 0.2f;
    bool facingRight = true;

    EnemyState state = ALIVE;
    float deathTimer = 0.0f;
    float deathFrameSpeed = 0.15f;

    //bool CheckCollisionWithMap(Map& map, Vector2 nextPos);
};

