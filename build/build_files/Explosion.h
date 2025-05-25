#pragma once

#include "raylib.h"


enum ExplosionType
{
	CENTER,
	MID_HORIZONTAL,
	MID_VERTICAL,
	UP,
	DOWN,
	LEFT,
	RIGHT
};

class Explosion
{
public:
	Explosion(Vector2 pos, ExplosionType type);	
	
	bool UpdateStatus(float deltaTime);
	void DrawExplosion(Texture2D explosionTexture) const;
	Rectangle GetExplosionRect() const;

	Vector2 pos;

private:
	float timer;

	ExplosionType type;
	//float animationTimer = 0.0f;
	int currentFrame = 0;
	float frameSpeed = 0.1f;
	float initialTimer;
};


