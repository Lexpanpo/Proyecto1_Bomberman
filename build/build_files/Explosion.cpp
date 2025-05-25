#include "Explosion.h"

Explosion::Explosion(Vector2 p):pos(p), timer(1.0f){}

bool Explosion::UpdateStatus(float deltaTime)
{
	timer -= deltaTime;

	return timer <= 0;
}

void Explosion::DrawExplosion(Texture2D explosion) const
{
	/*DrawRectangle(pos.x + 5, pos.y + 5, 30, 30, RED);*/
	Rectangle explosionRecorte = { sprite_status.x,sprite_status.y,80,80 };
	Rectangle explosionPosYtamaño = { pos.x, pos.y,40 , 40  };
	DrawTexturePro(explosion, explosionRecorte, explosionPosYtamaño, { 0,0 }, 0, WHITE);
}

Rectangle Explosion::GetExplosionRect() const
{
	return Rectangle{ pos.x + 5, pos.y + 5, 30, 30 };
}
