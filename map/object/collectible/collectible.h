#pragma once

class Collectible
{
public:
	enum CollectibleType
	{
		SILVER,
		GOLD,
		MONEYBAG,
		DIAMONDRING
	};
public:
	Collectible(const int2 loc, const CollectibleType type);
	~Collectible();

	void Draw(Surface* screen, const float2 offset);
	void UpdateFrame(const float deltaTime);
	bool CheckCollision(const float2 playerLoc, const int2 playerDimensions) const;
	void Reset();
	int PickUp();
private:
	int2 location;
	
	Sprite* collectibleSprite = nullptr;

	const CollectibleType TYPE;

	bool pickedUp = false;
	int worth = 0;

	int currentFrame = 0;
	float timeCounter = 0.0f;
	static constexpr float FRAMETIME = 0.05f;
};