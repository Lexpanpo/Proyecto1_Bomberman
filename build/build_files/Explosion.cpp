#include "Explosion.h"

Explosion::Explosion(Vector2 p, ExplosionType t):pos(p), type(t), timer(1.0f)
{
	frameSpeed = 0.1f;
	initialTimer = 7.0f * frameSpeed;
	timer = initialTimer;
	currentFrame = 0;
}

bool Explosion::UpdateStatus(float deltaTime)
{
	const int frameSequence[] = { 0, 1, 2, 3, 2, 1, 0 };
	const int totalPhases = 7;

	timer -= deltaTime;

	if (timer <= 0)
	{
		return true;
	}

	float timePassed = initialTimer - timer;

	int phase = timePassed / frameSpeed;

	if (phase < 0) phase = 0;
	if (phase >= totalPhases) phase = totalPhases - 1;

	currentFrame = frameSequence[phase];

	return false;

	/*animationTimer += deltaTime;
	
	if (animationTimer >= frameSpeed)
	{
		animationTimer = 0.0f;
		currentFrame = (currentFrame + 1) % 4;
	}*/
}

void Explosion::DrawExplosion(Texture2D explosionTexture) const
{
	int col = 0;

	switch (type)
	{
		case CENTER:     col = 0; break;
		case UP:         col = 1; break;
		case MID_VERTICAL:   col = 2; break;
		case MID_HORIZONTAL: col = 3; break;
		case RIGHT:      col = 4; break;
		case DOWN:       col = 6; break;
		case LEFT:       col = 7; break;
	}

	int row = currentFrame;

	Rectangle sourceRec = { col * 16, row * 16, 16, 16 };
	Rectangle destRec = { pos.x, pos.y, 40, 40 };

	DrawTexturePro(explosionTexture, sourceRec, destRec, { 0,0 }, 0, WHITE);
	//DrawRectangle(pos.x, pos.y, 40, 40, RED);
}

Rectangle Explosion::GetExplosionRect() const
{
	return Rectangle{ pos.x, pos.y, 40, 40 };
}