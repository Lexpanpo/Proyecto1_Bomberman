#pragma once

#include "raylib.h"

enum PowerUpType
{
	BOMBS_UP,
	FLAMES_UP,
	SPEED_UP,
	FLAME_PASS
};

struct PowerUp
{
	Vector2 pos;
	Rectangle rect;
	PowerUpType type;
	bool active = true;
};

