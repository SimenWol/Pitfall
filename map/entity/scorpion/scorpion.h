#pragma once

class Tilemap;

class Scorpion
{
public:
	Scorpion(float2 loc);
public:
	void Draw(Surface* screen, const float2 offset);
	void Update(const float playerMidX, const float deltaTime, Tilemap* map);
	void SetLoc(const float2 loc);
	bool CheckCollision(float2 playerLoc, int2 playerDimensions);
private:
	void UpdateFrame();
private:
	float2 location;
	Sprite scorpionSprite;

	// Step & Animation Timer.
	int currentFrame = 0;
	float timeCounter = 0.0f;
	const float FRAMETIME = 0.2f;

	bool facingRight = false;
	bool didNotMove = false;
};