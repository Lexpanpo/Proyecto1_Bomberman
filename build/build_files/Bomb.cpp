#include "Bomb.h"
#include "Explosion.h"

#include <iostream>
using namespace std;

extern vector<Explosion> explosions;

Bomb::Bomb(Vector2 pos)
{
	this->pos = pos;
	timer = 2.0f;
	hasExploded = false;

	animationTimer = 0.0f;
	currentFrame = 0;
	sprite_status = { 0, 0, 16, 16 };
}


bool Bomb::UpdateState(float deltaTime, Map& map)
{
	if (hasExploded)
	{
		return true;
	}

	const int bomb_sprites_x[] = { 0, 16, 32 };
	const int numFrames = 3;

	animationTimer += deltaTime;

	if (animationTimer >= frameSpeed)
	{
		animationTimer = 0.0f;
		currentFrame = (currentFrame + 1) % numFrames;
		sprite_status.x = (float)bomb_sprites_x[currentFrame];
	}

	timer -= deltaTime;

	if (timer <= 0)
	{
		Explode(map);
		hasExploded = true;
	}

	return hasExploded;
}

void Bomb::Explode(Map& map)
{
	//cout << "EXPLOTA" << endl;

	int centerX = pos.x / 40;
	int centerY = pos.y / 40;

	explosions.push_back(Explosion({ centerX * 40.0f, centerY * 40.0f }));

	int range = 1; // + RangeModifier;

	for (int dx = -1; dx <= 1; dx++)
	{
		if (dx == 0)
		{
			continue;
		}

		for (int i = 1; i <= range; i++)
		{
			int x = centerX + dx * i;

			if (map.BreakTile(x, centerY))
			{
				break;
			}

			explosions.push_back(Explosion({ x * 40.0f, centerY * 40.0f }));
		}
	}

	for (int dy = -1; dy <= 1; dy++)
	{
		if (dy == 0)
		{
			continue;
		}

		for (int i = 1; i <= range; i++)
		{
			int y = centerY + dy * i;

			if (map.BreakTile(centerX, y))
			{
				break;
			}

			explosions.push_back(Explosion({ centerX * 40.0f, y * 40.0f }));
		}
	}
}

void Bomb::DrawBomb(Texture2D bomba) const
{
	Rectangle bombaRecorte = { sprite_status.x,sprite_status.y,16,16 };
	Rectangle bombaPosYtamaño = { pos.x, pos.y, 20*2, 20*2 };
	DrawTexturePro(bomba, bombaRecorte, bombaPosYtamaño, { 0,0 }, 0, WHITE);

	DrawRectangleLines(pos.x, pos.y, 40, 40, YELLOW); //  Debug para ver los colliders de las bombas

}

//void Bomb::Update(float delta) const
//{
//	animationTimer += delta;
//
//	if (animationTimer >= framespeed)
//	{
//		currentFrame = (currentFrame + 1) % 2;
//		animationTimer = 0.0f;
//	}
//}
