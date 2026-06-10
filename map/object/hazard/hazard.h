#pragma once

class Hazard
{
public:
	enum HazardType
	{
		BONFIRE,
		SNAKE
	};
public:
	Hazard(const int2 loc, const HazardType type);
	~Hazard();

	void Draw(Surface* screen, const float2 offset);
	void UpdateFrame(const float deltaTime);
	bool CheckCollision(const float2 playerLoc, const int2 playerDimensions) const;
private:
	int2 location;
	Sprite* hazardSprite = nullptr;

	int currentFrame = 0;
	float timeCounter = 0.0f;
	float frameTime = 0.0f;
};