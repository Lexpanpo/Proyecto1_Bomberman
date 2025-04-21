#pragma once

#include "raylib.h"

class Map
{
public:
	Map();
	
	void DrawMap() const;

	bool CheckCollisions(Rectangle& playerRect);

	bool BreakTile(int gridX, int gridY);

	void PlaceSoftBlocks();

	bool IsDoorSpawned();
	Vector2 GetDoorPos();

private:
	int grid[13][31];

	bool doorSpawned;
	Vector2 doorPos;
};


