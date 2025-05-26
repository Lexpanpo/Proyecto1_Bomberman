#pragma once

#include "Map.h"
#include "Bomb.h"
#include "PowerUps.h"

#include <iostream>
#include <vector>
#include "raylib.h"

using namespace std;

enum PlayerState {
	P_ALIVE,
	P_DYING
};

class Player
{
public:
	Player();

	void UpdatePlayer(Map& map, float deltaTime ,static Sound soundArray[]);
	void DrawPlayer(Texture2D bomberman, Texture2D bomba);

	vector<Bomb> bombs;

	Vector2 GetPlayerPos();
	void SetPlayerPos();
	Rectangle GetPlayerRect();

	void TakeDamage();
	int GetCurrentHp() const;

	void PickPowerUp(PowerUpType type);
	int GetBombRange() const;
	bool HasFlamePass() const;

	PlayerState GetState() const;
	bool DeathAnimationFinished() const;

	bool victoryStatus();

private:
	Rectangle rect;
	Vector2 pos;
	float playerSpeed = 2.0f;

	int playerHp = 3;

	bool hasWin = false;

	int mirando = 0;
	Rectangle sprite_status{ 66,0 };
	float timerAnimacion = 0.0f;
	const float frameSpeed = 0.10f; //	Cambiar esto para ajustar la velocidad de la animacion
	int currentFrame = 1;
	int maxBombs = 1;

	int bombRange = 1;
	bool hasFlamePass = false;

	PlayerState state = P_ALIVE;
	float deathAnimTimer = 0.0f;
	int deathCurrentFrame = 0;
	float p_DeathFrameSpeed = 0.15f;
};

