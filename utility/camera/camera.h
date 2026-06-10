#pragma once

class Camera
{
public:
	float2 GetOffSet() const;
	void Reset();
	void Tick(const Surface* const screen, const float2 playerLoc, const float deltaTime);
private:
	float2 offset = { 0.0f, 0.0f };
};