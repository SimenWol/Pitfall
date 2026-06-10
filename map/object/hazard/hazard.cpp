#include "precomp.h"
#include "hazard.h"

Hazard::Hazard(const int2 loc, const HazardType type)
	:location(loc)
{
	switch (type)
	{
	case HazardType::BONFIRE:
		hazardSprite = new Sprite(new Surface("assets/Enemies/bonfiresheet.png"), 2);
		frameTime = 0.06f;
		break;
	case HazardType::SNAKE:
		hazardSprite = new Sprite(new Surface("assets/Enemies/snakesheet.png"), 2);
		frameTime = 0.05f;
		break;
	}
}

Hazard::~Hazard()
{
	delete hazardSprite;
}

void Hazard::Draw(Surface* screen, const float2 offset)
{
	hazardSprite->Draw(screen, location.x - static_cast<int>(offset.x), location.y - static_cast<int>(offset.y));
}

void Hazard::UpdateFrame(const float deltaTime)
{
	timeCounter += deltaTime;
	while (timeCounter > frameTime)
	{
		timeCounter -= frameTime;
		currentFrame++;
	}
	if (currentFrame >= 2) { currentFrame = 0; }
	hazardSprite->SetFrame(currentFrame);
}

bool Hazard::CheckCollision(const float2 playerLoc, const int2 playerDimensions) const
{
	// Simple box-box check
	if (playerLoc.x > location.x + hazardSprite->GetWidth() - 25 || playerLoc.x + playerDimensions.x < location.x + 25) { return false; } // check x overlap
	if (playerLoc.y > location.y + hazardSprite->GetHeight() || playerLoc.y + playerDimensions.y < location.y + 30) { return false; } // check y overlap

	return true; // Returns true if there is overlap
}
