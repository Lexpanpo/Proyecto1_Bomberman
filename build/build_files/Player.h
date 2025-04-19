#pragma once

#include "Map.h"

#include "raylib.h"

class Player
{
public:
	Player();

	void UpdatePlayer(Map& map);
	void DrawPlayer();

private:
	Rectangle rect;
	Vector2 pos;
	const float playerSpeed = 2.0f;
};

