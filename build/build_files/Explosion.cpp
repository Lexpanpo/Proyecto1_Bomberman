#include "Explosion.h"

Explosion::Explosion(Vector2 p):pos(p), timer(1.0f){}

bool Explosion::UpdateStatus(float deltaTime)
{
	timer -= deltaTime;

	return timer <= 0;
}

void Explosion::DrawExplosion() const
{
	DrawRectangle(pos.x + 5, pos.y + 5, 30, 30, RED);
}