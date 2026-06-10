#include "precomp.h"
#include "map/object/collectible/collectible.h"
#include "map/tilemap.h"

Collectible::Collectible(const int2 loc, const CollectibleType typeIn)
	:location(loc)
	,TYPE(typeIn)
{
	switch (typeIn)
	{
	case CollectibleType::DIAMONDRING:
		collectibleSprite = new Sprite(new Surface("assets/Collectibles/ring.png"), 1);
		worth = 5000;
		break;
	case CollectibleType::GOLD:
		collectibleSprite = new Sprite(new Surface("assets/Collectibles/gold.png"), 2);
		worth = 4000;
		break;
	case CollectibleType::SILVER:
		collectibleSprite = new Sprite(new Surface("assets/Collectibles/silver.png"), 2);
		worth = 3000;
		break;
	case CollectibleType::MONEYBAG:
		collectibleSprite = new Sprite(new Surface("assets/Collectibles/moneybag.png"), 1);
		worth = 2000;
		break;
	}

	// To ensure the sprite is drawn in the middle of a tile.
	location.x += static_cast<int>((Tilemap::TILESIZE - collectibleSprite->GetWidth()) / 2);
	location.y += static_cast<int>((Tilemap::TILESIZE - collectibleSprite->GetHeight()) / 2);
}

Collectible::~Collectible()
{
	delete collectibleSprite;
}

void Collectible::Draw(Surface* screen, const float2 offset)
{
	if (pickedUp) { return; }
	collectibleSprite->Draw(screen, location.x - static_cast<int>(offset.x), location.y - static_cast<int>(offset.y));
}

void Collectible::UpdateFrame(const float deltaTime)
{
	if (TYPE == CollectibleType::DIAMONDRING || TYPE == CollectibleType::MONEYBAG) { return; }

	timeCounter += deltaTime;
	while (timeCounter > FRAMETIME)
	{
		timeCounter -= FRAMETIME;
		currentFrame++;
		if (currentFrame >= 2) { currentFrame = 0; }
	}
	collectibleSprite->SetFrame(currentFrame);
}

bool Collectible::CheckCollision(const float2 playerLoc, const int2 playerDimensions) const
{
	// Simple box-box check
	if (playerLoc.x > location.x + collectibleSprite->GetWidth() || playerLoc.x + playerDimensions.x < location.x) { return false; } // check x overlap
	if (playerLoc.y > location.y + collectibleSprite->GetHeight() || playerLoc.y + playerDimensions.y < location.y) { return false; } // check y overlap

	return true; // Returns true if there is overlap
}

void Collectible::Reset()
{
	pickedUp = false;
}

int Collectible::PickUp()
{
	if (pickedUp) { return 0; }

	pickedUp = true;
	return worth;
}