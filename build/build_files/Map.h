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

	void ClearMap();
	//void ResetLevel();

private:
	int grid[13][31];
	Rectangle rect;

	bool doorSpawned; 
	Vector2 doorPos;

	Texture2D solidBlock = LoadTexture("resources/bombermanSprites/General Sprites/Walls_Sprites.png");
};


