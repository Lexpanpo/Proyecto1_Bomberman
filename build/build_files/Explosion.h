#pragma once

#include "raylib.h"


class Explosion
{
public:
	Explosion(Vector2 pos);
	
	bool UpdateStatus(float deltaTime);
	void DrawExplosion() const;
	Rectangle GetExplosionRect() const;

	Vector2 pos;

private:
	float timer;
};


