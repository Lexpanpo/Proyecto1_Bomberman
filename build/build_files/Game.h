#pragma once

#include "raylib.h"

class Game
{
public:
	Game();
	~Game();

	void Run();

private:
	Camera2D camera;
};

