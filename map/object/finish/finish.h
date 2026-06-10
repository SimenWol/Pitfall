#pragma once

class Finish
{
public:
	Finish(const int2 loc);

	void Draw(Surface* screen, const float2 offset);
	void UpdateFrame(const float deltaTime);
	bool CheckCollision(const float2 playerLoc, const int2 playerDimensions);
private:
	int2 location;
	Sprite finishSprite;

	int currentFrame = 0;
	float timeCounter = 0.0f;
	static constexpr float FRAMETIME = 0.1f;
};