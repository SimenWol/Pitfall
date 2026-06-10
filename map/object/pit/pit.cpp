#include "precomp.h"
#include "map/object/pit/pit.h"
#include "map/object/alligator/alligator.h"
#include "map/tilemap.h"

Pit::Pit(int2 loc, bool shrinking, bool water)
	:location(loc)
	,shrinks(shrinking)
	,waterFilled(water)
{
	currentSize = MAX_SIZE * Tilemap::TILESIZE;
	if (waterFilled)
	{
		colour = 0x386890;

		alligators[0] = new Alligator({ static_cast<int>(loc.x - 1.75 * Tilemap::TILESIZE), loc.y - Alligator::HEIGHT + 5 });
		alligators[1] = new Alligator({ static_cast<int>(loc.x + 0.75 * Tilemap::TILESIZE), loc.y - Alligator::HEIGHT + 5 });
	}
	else { colour = 0x000000; }
}

void Pit::Update(const float deltaTime)
{
	if (waterFilled)
	{
		alligators[0]->Update(deltaTime);
		alligators[1]->Update(deltaTime);
		return;
	}
	else if (!shrinks) { return; }

	if (timer != 0.0f)
	{
		timer -= deltaTime;
		if (timer <= 0.0f)
		{
			timer = 0.0f;
			shrinking = !shrinking;
		}
	}
	else if (shrinking)
	{
		currentSize -= deltaTime * 450;
		if (currentSize <= 0)
		{
			timer += shrinkWaitTime;
			currentSize = 0;
		}
	}
	else
	{
		currentSize += deltaTime * 450;
		if (currentSize >= MAX_SIZE * Tilemap::TILESIZE)
		{
			timer += shrinkWaitTime + 1.0f; // Add extra time when pit is expanded
			currentSize = MAX_SIZE * Tilemap::TILESIZE;
		}
	}
}


void Pit::Draw(Surface* screen, const float2 offset) const
{
	if (currentSize == 0.0f) { return; }

	const int yShrink = static_cast<int>(currentSize / 32);

	screen->Bar(location.x - static_cast<int>(currentSize) - static_cast<int>(offset.x), location.y - static_cast<int>(yShrink * 0.5f) - static_cast<int>(offset.y),
		location.x + static_cast<int>(currentSize) - static_cast<int>(offset.x), location.y + static_cast<int>(yShrink * 0.5f) - static_cast<int>(offset.y), colour);

	screen->Bar(location.x - static_cast<int>(currentSize) + 30 - static_cast<int>(offset.x), location.y - 2 - yShrink - static_cast<int>(offset.y),
		location.x + static_cast<int>(currentSize) - 30 - static_cast<int>(offset.x), location.y + 2 + yShrink - static_cast<int>(offset.y), colour);

	screen->Bar(location.x - static_cast<int>(currentSize) + 60 - static_cast<int>(offset.x), location.y - 1 - yShrink * 2 - static_cast<int>(offset.y),
		location.x + static_cast<int>(currentSize) - 60 - static_cast<int>(offset.x), location.y + 1 + yShrink * 2 - static_cast<int>(offset.y), colour);

	screen->Bar(location.x - static_cast<int>(currentSize) + 110 - static_cast<int>(offset.x), location.y - yShrink * 3 - static_cast<int>(offset.y),
		location.x + static_cast<int>(currentSize) - 110 - static_cast<int>(offset.x), location.y + yShrink * 3 - static_cast<int>(offset.y), colour);

	screen->Bar(location.x - static_cast<int>(currentSize) + 170 - static_cast<int>(offset.x), location.y - 5 - yShrink * 3 - static_cast<int>(offset.y),
		location.x + static_cast<int>(currentSize) - 170 - static_cast<int>(offset.x), location.y + 5 + yShrink * 3 - static_cast<int>(offset.y), colour);

	if (waterFilled)
	{
		alligators[0]->Draw(screen, offset);
		alligators[1]->Draw(screen, offset);
	}
}

bool Pit::CheckCollision(const float2 playerLoc, const int2 playerDimensions)
{
	if (waterFilled && alligators[0]->GetMouthClosed())
	{
		playerOffset.y += alligators[0]->CheckCollision(playerLoc, playerDimensions);
		playerOffset.y += alligators[1]->CheckCollision(playerLoc, playerDimensions);
		if (playerOffset.y != 0.0f) { return false; }
	}

	// Simple box-box check
	if (playerLoc.y + playerDimensions.y > location.y + 4 || playerLoc.y + playerDimensions.y < location.y) { return false; } // check y overlap

	if (playerLoc.x < location.x - static_cast<int>(currentSize)) { return false; }
	if (playerLoc.x + playerDimensions.x > location.x + static_cast<int>(currentSize)) { return false; }

	return true; // Returns true if there is overlap
}

float2 Pit::GetPlayerOffset()
{
	float2 temp = playerOffset;
	playerOffset = 0.0f;
	return temp;
}

void Pit::Reset()
{
	timer = 0.0f;
	if (shrinks) { currentSize = MAX_SIZE; }
}
