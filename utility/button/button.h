#pragma once

class Button
{
public:
	Button(Sprite& sprite_in, const int2 loc = { 0, 0 });

	void SetLocation(const int2 loc);
	bool IsPressed(const float2 mousePos, const bool mouseDown);
	void Draw(Surface* screen, const float2 mousePos);
private:
	bool CheckMousePos(const float2 mousePos) const;
	bool CheckMouseDown(const bool mouseDown);
private:
	int2 location;
	int2 dimensions;

	bool prevMouseDown = false;

	Sprite& sprite;
};