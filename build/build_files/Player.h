#pragma once

#include "Map.h"
#include "Bomb.h"

#include <iostream>
#include <vector>
#include "raylib.h"

using namespace std;



class Player
{
public:
	Player();

	void UpdatePlayer(Map& map);
	void DrawPlayer();

	vector<Bomb> bombs;

	Vector2 GetPlayerPos();

private:
	Rectangle rect;
	Vector2 pos;
	const float playerSpeed = 2.0f;
};

