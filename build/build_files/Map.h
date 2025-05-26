#pragma once

#include "PowerUps.h"

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

	bool CheckCollisions(Rectangle& playerRect, bool ignoreSoftB = false);

	bool BreakTile(int gridX, int gridY);

	void PlaceSoftBlocks();

	bool IsTileEmpty(int gridX, int gridY) const;
	int GetTileType(int gridX, int gridY) const;

	void DrawPowerUps(Texture2D powerUpTexture) const;
	vector<PowerUp>& GetPowerUps();
	void RemovePowerUpAt(int index);

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
	float doorSpawnChance = 0.2f; // 0.2 = 20% || Modificar para ajustarlo

	vector<AnimatingBlock> animatingBlocks;
	float blockFrameSpeed = 0.1f;

	vector<PowerUp> activePowerUps;
	int powerUpsSpawned = 0;
	const int maxPowerUps = 3; // Maximo de PowerUps por nivel
	float powerUpsSpawnChance = 0.3f; // 0.3 = 30% || Modificar para ajustarlo

};


