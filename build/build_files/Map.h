#pragma once

#include <vector>
#include "raylib.h"

using namespace std;

struct AnimatingBlock
{
	Vector2 pos;
	float timer = 0.0f;
	int frame = 0;
};

class Map
{
public:
	Map();
	
	void DrawMap(Texture2D walls) const;

	bool CheckCollisions(Rectangle& playerRect);

	bool BreakTile(int gridX, int gridY);

	void PlaceSoftBlocks();

	bool IsTileEmpty(int gridX, int gridY) const;
	int GetTileType(int gridX, int gridY) const;

	bool IsDoorSpawned();
	Vector2 GetDoorPos();
	void SpawnDoor();

	void ClearMap();
	void ResetLevel();

	void Update(float deltaTime);

private:
	int grid[13][31];
	Rectangle rect;

	bool doorSpawned; 
	Vector2 doorPos;
	Rectangle sprite_status{ 16,0 };

	vector<AnimatingBlock> animatingBlocks;
	float blockFrameSpeed = 0.1f;

};


