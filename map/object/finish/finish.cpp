#include "precomp.h"
#include "finish.h"

Finish::Finish(const int2 loc)
	:location(loc)
	,finishSprite(new Surface("assets/Level/flag.png"), 9)
{}

void Finish::Draw(Surface * screen, const float2 offset)
{
	finishSprite.Draw(screen, location.x - static_cast<int>(offset.x), location.y - static_cast<int>(offset.y));
}

void Finish::UpdateFrame(const float deltaTime)
{
	timeCounter += deltaTime;
	while (timeCounter > FRAMETIME)
	{
		timeCounter -= FRAMETIME;
		currentFrame++;
		if (currentFrame >= 9) { currentFrame = 0; }
	}
	finishSprite.SetFrame(currentFrame);
}

bool Finish::CheckCollision(const float2 playerLoc, const int2 playerDimensions)
{
	// Simple box-box check 1
	if (playerLoc.x > location.x + finishSprite.GetWidth() - 42 || playerLoc.x + playerDimensions.x < location.x + 15) {} // check x overlap
	else if (playerLoc.y > location.y + finishSprite.GetHeight() || playerLoc.y + playerDimensions.y < location.y) {} // check y overlap
	else { return true; }

	// Simple box-box check 2 (more accurate approximate shape)
	if (playerLoc.x > location.x + finishSprite.GetWidth() - 15 || playerLoc.x + playerDimensions.x < location.x + 21) { return false; } // check x overlap
	if (playerLoc.y > location.y + finishSprite.GetHeight() - 28 || playerLoc.y + playerDimensions.y < location.y + 10) { return false; } // check y overlap
	return true; // Returns true if there is overlap
}

