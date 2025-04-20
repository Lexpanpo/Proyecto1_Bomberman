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

private:
	int grid[13][31];
};


