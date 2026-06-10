#pragma once

class Rope
{
public:
	Rope(const float2 loc, const float length);
	void Draw(Surface* screen, const float2 offset);
	void Update(const float deltaTime);
	float2 GetTopLoc() const;
	float2 GetBottomLoc() const;
	void Reset();
private:
	float2 topLoc;
	float2 bottomLoc;
	float ropeLength;

	float timeElapsed = 0.0f;

	float swingSpeed = 1.15f;
	float swingStrength = 0.7f;
};