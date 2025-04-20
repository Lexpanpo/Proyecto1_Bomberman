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
}


bool Bomb::UpdateState(float deltaTime, Map& map)
{
	if (hasExploded)
	{
		return true;
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

void Bomb::DrawBomb() const 
{
	DrawRectangle(pos.x + 20, pos.y + 20, 12, 12, BLACK);
}