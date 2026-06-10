#pragma once

class Alligator
{
public:
	Alligator(int2 loc);
public:
	void Draw(Surface* screen, const float2 offset);
	void Update(const float deltaTime);
	float CheckCollision(const float2 playerLoc, const int2 playerDimensions) const;
	bool GetMouthClosed() const;
private:
	void AnimateAlligator();
public:
	const int2 location;
	static constexpr int WIDTH = 72;
	static constexpr int HEIGHT = 36;

	Sprite alligatorSprite;

	float timer = 0.0f;
	float waitTime = 2.0f;
	bool mouthClosed = false;
};