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

   
    void Move(Map& map, float deltaTime, const vector<Bomb>& playerBombs);
    void ChangeDirection();

    //bool CheckCollisionWithMap(Map& map, Vector2 nextPos);

};

