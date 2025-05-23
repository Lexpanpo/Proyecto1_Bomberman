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

	void DrawBomb(Texture2D bomba) const;

	void Update(float delta) const;

private:
	Rectangle rect;
	Vector2 pos;
	float timer;
	bool hasExploded;
	float animationTimer = 0.0f;
	int CurrentFrame = 0;
	Rectangle sprite_status{ 16,0 };


	void Explode(Map& map);
};
