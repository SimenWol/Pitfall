#include "precomp.h"
#include "alligator.h"

Alligator::Alligator(int2 loc)
	:location(loc)
	,alligatorSprite(new Surface("assets/enemies/crocsheet.png"), 2)
{}

void Alligator::Draw(Surface * screen, const float2 offset)
{
	alligatorSprite.Draw(screen, static_cast<int>(location.x - offset.x), static_cast<int>(location.y - offset.y));
}

void Alligator::Update(const float deltaTime)
{
	timer += deltaTime;
	while (timer > waitTime)
	{
		timer -= waitTime;
		mouthClosed = !mouthClosed;
		AnimateAlligator();
	}

}

float Alligator::CheckCollision(const float2 playerLoc, const int2 playerDimensions) const
{
	if (playerLoc.x + playerDimensions.x < location.x || playerLoc.x > location.x + WIDTH) { return 0.0f; }
	if (playerLoc.y + playerDimensions.y < location.y + 20 || playerLoc.y > location.y + HEIGHT) { return 0.0f; }

	return static_cast<float>(location.y) + 20.0f - (playerLoc.y + playerDimensions.y);
}

bool Alligator::GetMouthClosed() const { return mouthClosed; }

void Alligator::AnimateAlligator()
{
	if (mouthClosed) { alligatorSprite.SetFrame(0); }
	else { alligatorSprite.SetFrame(1); }
}
