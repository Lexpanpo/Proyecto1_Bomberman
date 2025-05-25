#include "Bomb.h"
#include "Explosion.h"

#include <iostream>
using namespace std;

extern vector<Explosion> explosions;

Bomb::Bomb(Vector2 pos, int bombRange)
{
	this->pos = pos;
	this->range = bombRange;
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
	int range = 1; // + RangeModifier;

	explosions.push_back(Explosion({ centerX * 40.0f, centerY * 40.0f }, ExplosionType::CENTER));

	int dirs[4][4] = {{1, 0, ExplosionType::RIGHT, ExplosionType::MID_HORIZONTAL},
					  {-1, 0, ExplosionType::LEFT, ExplosionType::MID_HORIZONTAL},
					  {0, 1, ExplosionType::DOWN, ExplosionType::MID_VERTICAL},
					  {0, -1, ExplosionType::UP, ExplosionType::MID_VERTICAL}};

	for (int d = 0; d < 4; d++)
	{
		for (int i = 1; i <= range; i++)
		{
			int x = centerX + dirs[d][0] * i;
			int y = centerY + dirs[d][1] * i;

			int tileType = map.GetTileType(x, y);

			if (tileType == 1)
			{
				break;
			}
			
			if (tileType == 2)
			{
				map.BreakTile(x, y);
				break;
			}

			ExplosionType endType = (ExplosionType)dirs[d][2];
			ExplosionType midType = (ExplosionType)dirs[d][3];

			int nextX = centerX + dirs[d][0] * (i + 1);
			int nextY = centerY + dirs[d][1] * (i + 1);
			bool isLastPiece = (i == range) || (map.GetTileType(nextX, nextY) == 1);

			ExplosionType currentType = isLastPiece ? endType : midType;
			explosions.push_back(Explosion({ x * 40.0f, y * 40.0f }, currentType));

			
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

Rectangle Bomb::GetRect() const
{
	return { pos.x, pos.y, 40.0f, 40.0f };
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
