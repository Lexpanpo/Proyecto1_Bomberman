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
	Bomb(Vector2 pos, int range = 2); // Rango 2 por defecto, cambiar luego si añadimos el modificador

	bool UpdateState(float deltaTime, Map& map);

	void DrawBomb(Texture2D bomba) const;

	Rectangle GetRect() const;

	//void Update(float delta) const;

private:

	Vector2 pos;
	float timer;
	int range;
	bool hasExploded;

	float animationTimer = 0.0f;
	int currentFrame = 0;
	Rectangle sprite_status { 0, 0, 16, 16};
	float frameSpeed = 0.3f;

	void Explode(Map& map);
};
