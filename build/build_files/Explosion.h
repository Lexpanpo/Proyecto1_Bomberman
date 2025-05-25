#pragma once

#include "raylib.h"


class Explosion
{
public:
	Explosion(Vector2 pos);
	
	bool UpdateStatus(float deltaTime);
	void DrawExplosion(Texture2D explosion) const;
	Rectangle GetExplosionRect() const;

	Vector2 pos;

private:
	float timer;

	float animationTimer = 0.0f;
	int currentFrame = 0;
	Rectangle sprite_status{ 0, 0, 80, 80 };
	float frameSpeed = 0.3f;
};


