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

class Enemy
{
public:
	Enemy(Vector2 startPos, EnemyType type);

	void Update(Map& map, float deltaTime, const vector<Bomb>& playerBombs);

	void Draw()const;

	Rectangle GetRect() const;

    EnemyType GetType() const;
    int GetScoreValue() const;

	bool IsAlive() const;
	void Kill();

private:
    Vector2 pos;
    Rectangle rect;

    float speed;
    int direction;
    float moveTimer;

    bool alive;
   
    void Move(Map& map, float deltaTime, const vector<Bomb>& playerBombs);
    void ChangeDirection();

    EnemyType type;
    int scoreValue;

    //bool CheckCollisionWithMap(Map& map, Vector2 nextPos);
};

