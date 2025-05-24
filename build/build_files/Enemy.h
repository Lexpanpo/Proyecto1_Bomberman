#pragma once

#include "raylib.h"
#include "Map.h"

class Enemy
{
public:
	Enemy(Vector2 startPos);

	void Update(Map& map, float deltaTime);

	void Draw()const;

	Rectangle GetRect() const;

	bool IsAlive() const;
	void Kill();

private:
    Vector2 pos;
    Rectangle rect;

    float speed;

    bool alive;

    int direction;

    float moveTimer;

   
    void Move(Map& map, float deltaTime);
    void ChangeDirection();

    bool CheckCollisionWithMap(Map& map, Vector2 nextPos);

};

