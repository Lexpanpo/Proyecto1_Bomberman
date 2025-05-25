#pragma once

#include "Map.h"
#include "Bomb.h"

#include <vector>
#include "raylib.h"

using namespace std;

class Enemy
{
public:
	Enemy(Vector2 startPos);

	void Update(Map& map, float deltaTime, const vector<Bomb>& playerBombs);

	void DrawEnemy(Texture2D enemy) const;

	Rectangle GetRect() const;

	bool IsAlive() const;
	void Kill();

private:
    Vector2 pos;
    Rectangle rect;

    float speed;
    float timer;
    bool alive;

    int direction;

    float moveTimer;

    float animationTimer = 0.0f;
    int currentFrame = 0;
    Rectangle sprite_status{ 0, 0, 16, 16 };
    float frameSpeed = 0.3f;

    void Move(Map& map, float deltaTime, const vector<Bomb>& playerBombs);
    void ChangeDirection();

    //bool CheckCollisionWithMap(Map& map, Vector2 nextPos);

};

