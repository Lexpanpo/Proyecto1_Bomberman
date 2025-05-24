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
	void DrawPlayer(Texture2D bomberman, Texture2D bomba);

	vector<Bomb> bombs;

	Vector2 GetPlayerPos();

	void SetPlayerPos();

	Rectangle GetPlayerRect();

	void TakeDamage();
	bool wasHit = false;
	int GetCurrentHp() const;

	bool victoryStatus();
private:
	Rectangle rect;
	Vector2 pos;
	const float playerSpeed = 2.0f;

	int playerHp = 3;

	bool hasWin = false;

	int mirando = 0;
	Rectangle sprite_status{ 66,0 };
	float timerAnimacion = 0.0f;
	const float frameSpeed = 0.15f; //	Cambiar esto para ajustar la velocidad de la animacion
	int currentFrame = 1;
};

