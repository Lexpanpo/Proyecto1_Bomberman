#pragma once

#include "raylib.h"

class Map
{
public:
	Map();
	
	void DrawMap(Texture2D walls) const;

	bool CheckCollisions(Rectangle& playerRect);

	bool BreakTile(int gridX, int gridY);

	void PlaceSoftBlocks();

	bool IsDoorSpawned();
	Vector2 GetDoorPos();

	void ClearMap();
	//void ResetLevel();

private:
	int grid[13][31];
	Rectangle rect;

	bool doorSpawned; 
	Vector2 doorPos;
	Rectangle sprite_status{ 16,0 };

};


