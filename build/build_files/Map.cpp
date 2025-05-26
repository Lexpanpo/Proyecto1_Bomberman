#include "Map.h"

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "raylib.h"

using namespace std;

Map::Map()
{
	srand((unsigned int)time(nullptr));

	for (int y = 0; y < 13; y++)
	{
		for (int x = 0; x < 31; x++)
		{
			if (y == 0 || y == 12 || x == 0 || x == 30 || (y % 2 == 0 && x % 2 == 0)) 
			{
				grid[y][x] = 1;				
			}
			else 
			{
				grid[y][x] = 0;
			}
		}
	}

	PlaceSoftBlocks();

	doorSpawned = false;
	doorPos = { -1, -1 };
}

void Map::DrawMap(Texture2D walls) const
{
	Rectangle hardBlocksRecorte = { 0, 0 ,16,16 };
	Rectangle softBlocksRecorte = { 16, 0 ,16,16 };
	Rectangle doorRecorte = { 128, 0 ,16,16 };
	
	Vector2 origen = { 0, 0 };

	for (int y = 0; y < 13; y++)
	{
		for (int x = 0; x < 31; x++)
		{
			Rectangle dest = { x * 40, y * 40 ,40 ,40 };

			DrawRectangle(x * 40, y * 40, 40, 40, DARKGREEN);


			if (grid[y][x] == 1)
			{				
				//Color color = GRAY;
				DrawTexturePro(walls, hardBlocksRecorte, dest, origen, 0, WHITE);
			}
			else if (grid[y][x] == 2)
			{
				//Color color = LIGHTGRAY;
				DrawTexturePro(walls, softBlocksRecorte, dest, origen, 0, WHITE);
			}
			else if(grid[y][x] == 3)
			{
				bool found = false;

				for (const AnimatingBlock& block : animatingBlocks) 
				{
					if ((int)block.pos.x == x && (int)block.pos.y == y) 
					{						
						Rectangle animRec = { 16.0f + block.frame * 16.0f, 0, 16, 16 };
						DrawTexturePro(walls, animRec, dest, origen, 0, WHITE);
						found = true;
						break;
					}
				}
				if (!found) 
				{
					DrawTexturePro(walls, softBlocksRecorte, dest, origen, 0, WHITE);
				}
			}
		}
	}

	if (doorSpawned)
	{
		Rectangle doorDestRec = { doorPos.x, doorPos.y, 40, 40 };
		DrawTexturePro(walls, doorRecorte, doorDestRec, origen, 0, WHITE);
		//DrawRectangleV(doorPos, { 40,40 }, BLUE);
	}
}

bool Map::CheckCollisions(Rectangle& playerRect, bool ignoreSoftB)
{
	for (int y = 0; y < 13; y++)
	{
		for (int x = 0; x < 31; x++)
		{
			int tileType = grid[y][x];
			bool isSolid = false;

			if (tileType == 1)
			{
				isSolid = true;
			}
			else if (!ignoreSoftB && (tileType == 2 || tileType == 3))
			{
				isSolid = true;
			}

			if (isSolid)
			{
				Rectangle tileRect = { x * 40, y * 40, 40, 40 };

				if (CheckCollisionRecs(playerRect, tileRect))
				{
					return true;
				}
			}
		}
	}

	return false;
}

bool Map::BreakTile(int gridX, int gridY)
{
	if (gridX < 0 || gridX >= 31 || gridY < 0 || gridY >= 13)
	{
		return true;
	}

	if (grid[gridY][gridX] == 1)
	{
		return true;
	}

	if (grid[gridY][gridX] == 3)
	{
		return true;
	}

	if (grid[gridY][gridX] == 2)
	{
		bool spawnedSomething= false;

		if (!doorSpawned && GetRandomValue(0, 99) < (doorSpawnChance * 100))
		{
			grid[gridY][gridX] = 0;

			doorPos = { gridX * 40.0f, gridY * 40.0f };
			doorSpawned = true;

			spawnedSomething = true;
		}
		if (!spawnedSomething && powerUpsSpawned < maxPowerUps && GetRandomValue(0, 99) < (powerUpsSpawnChance * 100))
		{
			PowerUp newPowerUp;
			newPowerUp.type = (PowerUpType)GetRandomValue(0, 3);
			newPowerUp.rect = { gridX * 40.0f, gridY * 40.0f, 40.0f, 40.0f };
			newPowerUp.active = true;
			activePowerUps.push_back(newPowerUp);
			powerUpsSpawned++;
		}

		grid[gridY][gridX] = 3;
		animatingBlocks.push_back({ (float)gridX, (float)gridY });
		return true;
	}
	return false;
}

void Map::PlaceSoftBlocks()
{
	for (int y = 0; y < 13; y++)
	{
		for (int x = 0; x < 31; x++)
		{
			if (grid[y][x] == 0)
			{
				if (x <= 2 && y <= 2)
				{
					continue;
				}

				if (rand() % 100 < 40)
				{
					grid[y][x] = 2;
				}
			}
		}
	}
}

bool Map::IsTileEmpty(int gridX, int gridY) const
{
	if (gridX < 0 || gridX >= 31 || gridY < 0 || gridY >= 13)
	{
		return false; 
	}

	return grid[gridY][gridX] == 0;
}

int Map::GetTileType(int gridX, int gridY) const
{
	if (gridX < 0 || gridX >= 31 || gridY < 0 || gridY >= 13)
	{
		return 1;
	}

	return grid[gridY][gridX];
}

void Map::DrawPowerUps(Texture2D powerUpTexture) const
{
	for (const PowerUp& p : activePowerUps)
	{
		if (p.active)
		{
			Rectangle sourceRec = { p.type * 16, 0, 16, 16 };
			DrawTexturePro(powerUpTexture, sourceRec, p.rect, { 0,0 }, 0, WHITE);
		}
	}
}

vector<PowerUp>& Map::GetPowerUps()
{
	return activePowerUps;
}

void Map::RemovePowerUpAt(int index)
{
	if (index >= 0 && index < activePowerUps.size())
	{
		activePowerUps.erase(activePowerUps.begin() + index);
	}
}

bool Map::IsDoorSpawned()
{
	return doorSpawned;
}

Vector2 Map::GetDoorPos()
{
	return doorPos;
}

void Map::SpawnDoor()
{
	if (doorSpawned) return;

	vector<Vector2> softBlocks;

	for (int y = 0; y < 13; y++)
	{
		for (int x = 0; x < 31; x++) 
		{
			if (grid[x][y] == 2)
			{
				softBlocks.push_back({ (float)x, (float)y });
			}
		}
	}

	if (!softBlocks.empty())
	{
		Vector2 chosenBlock = softBlocks[GetRandomValue(0, softBlocks.size() - 1)];

		grid[(int)chosenBlock.x][(int)chosenBlock.y] = 0;
		doorPos = { chosenBlock.x * 40, chosenBlock.y * 40 };

		doorSpawned = true;
		cout << "SE HA GENERADO LA PERTA EN: " << doorPos.x << ", " << doorPos.y << endl;
	}
	else
	{
		cout << "No hay softblocks para generar la puerta!! Se ha colocado una en (1,3)" << endl;
		doorPos = { 1 * 40.0f, 3 * 40.0f };
		doorSpawned = true;
	}
	
}

void Map::ClearMap()
{
	for (int y = 0; y < 13; y++)
	{
		for (int x = 0; x < 31; x++)
		{
			if (grid[y][x] == 2)
			{
				BreakTile(x, y);
				//grid[y][x] = 0;

				//cout << "casilla eliminada";
			}
		}
	}
}

void Map::ResetLevel()
{
	doorSpawned = false;
	doorPos = { -1, -1 };

	activePowerUps.clear();
	powerUpsSpawned = 0;
	animatingBlocks.clear();

	for (int y = 0; y < 13; y++)
	{
		for (int x = 0; x < 31; x++)
		{
			if (y == 0 || y == 12 || x == 0 || x == 30 || (y % 2 == 0 && x % 2 == 0))
			{
				grid[y][x] = 1;
			}
			else
			{
				grid[y][x] = 0;
			}
		}
	}

	PlaceSoftBlocks();
}

void Map::Update(float deltaTime)
{
	for (int i = 0; i < animatingBlocks.size(); )
	{
		animatingBlocks[i].timer += deltaTime;
		
		if (animatingBlocks[i].timer >= blockFrameSpeed)
		{
			animatingBlocks[i].timer = 0.0f;
			animatingBlocks[i].frame++;
		}

		if (animatingBlocks[i].frame >= 7) 
		{
			int x = animatingBlocks[i].pos.x;
			int y = animatingBlocks[i].pos.y;

			grid[y][x] = 0;

			animatingBlocks.erase(animatingBlocks.begin() + i);
		}
		else
		{
			i++; 
		}
	}
}
