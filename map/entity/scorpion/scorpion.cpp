#include "precomp.h"
#include "map/entity/scorpion/scorpion.h"
#include "map/tilemap.h"

Scorpion::Scorpion(float2 loc)
	:scorpionSprite(new Surface("assets/Enemies/scorpionsheet.png"), 4)
	,location(loc)
{}

void Scorpion::Draw(Surface* screen, const float2 offset)
{
	scorpionSprite.Draw(screen, static_cast<int>(location.x - offset.x), static_cast<int>(location.y - offset.y));
}

void Scorpion::Update(const float playerMidX, const float deltaTime, Tilemap* map)
{
	timeCounter += deltaTime;

	while (timeCounter >= FRAMETIME)
	{
		timeCounter -= FRAMETIME;

		float scorpionMidX = static_cast<float>(location.x + 0.5 * scorpionSprite.GetWidth());

		if (playerMidX - scorpionMidX <= 10.0f && playerMidX - scorpionMidX >= -10.0f) { didNotMove = true; }
		else if (playerMidX < scorpionMidX)
		{
			location.x -= 0.1f * Tilemap::TILESIZE;
			facingRight = false;

			if (map->GetContents({ static_cast<int>(location.x), static_cast<int>(location.y) }) == Tilemap::TileContents::Obstacle)
			{
				location.x += Tilemap::TILESIZE - static_cast<int>(location.x) % Tilemap::TILESIZE;
				didNotMove = true;
			}
		}
		else if (playerMidX > scorpionMidX)
		{
			location.x += 0.1f * Tilemap::TILESIZE;
			facingRight = true;

			if (map->GetContents({ static_cast<int>(location.x + scorpionSprite.GetWidth()), static_cast<int>(location.y) }) == Tilemap::TileContents::Obstacle)
			{
				location.x -= static_cast<int>(location.x + scorpionSprite.GetWidth()) % Tilemap::TILESIZE;
				didNotMove = true;
			}
		}

		UpdateFrame();
	}
}

void Scorpion::SetLoc(const float2 loc)
{
	location = loc;
}

bool Scorpion::CheckCollision(float2 playerLoc, int2 playerDimensions)
{
	// Simple box-box check
	if (playerLoc.x > location.x + scorpionSprite.GetWidth() - 15 || playerLoc.x + playerDimensions.x < location.x + 15) { return false; } // check x overlap
	if (playerLoc.y > location.y + scorpionSprite.GetHeight() || playerLoc.y + playerDimensions.y < location.y + 24) { return false; } // check y overlap

	return true; // Returns true if there is overlap
}

void Scorpion::UpdateFrame()
{
	if (didNotMove) { didNotMove = false; return; }

	currentFrame++;
	if (currentFrame >= 2) { currentFrame = 0; }

	if (facingRight) { scorpionSprite.SetFrame(currentFrame); }
	else { scorpionSprite.SetFrame(currentFrame + 2); }
}
