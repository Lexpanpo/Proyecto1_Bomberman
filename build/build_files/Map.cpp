#include "Map.h"

#include <iostream>
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

}

void Map::DrawMap() const 
{
	for (int y = 0; y < 13; y++)
	{
		for (int x = 0; x < 31; x++)
		{
			if (grid[y][x] == 1)
			{
				Color color = GRAY;
				DrawRectangle(x * 40, y * 40, 40, 40, color);
			}
			else if (grid[y][x] == 2)
			{
				Color color = LIGHTGRAY;
				DrawRectangle(x * 40, y * 40, 40, 40, color);
			}
			else
			{
				Color color = DARKGREEN;
				DrawRectangle(x * 40, y * 40, 40, 40, color);
			}
		}
	}
}

bool Map::CheckCollisions(Rectangle& playerRect)
{
	for (int y = 0; y < 13; y++)
	{
		for (int x = 0; x < 31; x++)
		{
			if (grid[y][x] == 1 || grid[y][x] == 2)
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

bool Map::BreakTile(int posX, int posY)
{
	if (posX < 0 || posX >= 20 || posY < 0 || posY >= 15)
	{
		return true;
	}

	if (grid[posX][posY] == 2)
	{
		grid[posX][posY] == 0;

		return false;
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
