#pragma once

#include "Map.h"
#include "Explosion.h"

#include <iostream>
#include <vector>
#include "raylib.h"

using namespace std;

extern vector<Explosion> explosions;

class Bomb
{
public:
	Bomb(Vector2 pos);

	bool UpdateState(float deltaTime, Map& map);
	void DrawBomb() const;

private:
	Vector2 pos;
	float timer;
	bool hasExploded;

	void Explode(Map& map);
};
