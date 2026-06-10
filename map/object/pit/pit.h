#pragma once

class Alligator;

class Pit
{
public:
	Pit(int2 loc, bool shrinking, bool water);
public:
	void Draw(Surface* screen, const float2 offset) const;
	void Update(const float deltaTime);
	bool CheckCollision(const float2 playerLoc, const int2 playerDimensions);
	float2 GetPlayerOffset();
	void Reset();
private:
	int2 location;
	float2 playerOffset = 0.0f;

	static constexpr int MAX_SIZE = 3; // Size in one direction (this means the hole can only expand x tiles to either side).
	float currentSize = 0.0f;

	float timer = 0.0f;
	const float shrinkWaitTime = 1.5f;
	bool shrinking = true;

	const bool shrinks;
	const bool waterFilled;
	int colour;

	Alligator* alligators[2];
};