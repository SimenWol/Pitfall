#pragma once

class Background
{
public:
	Background();
public:
	void Draw(Surface* screen, const float2 offset);
private:
	Surface background;
	Sprite mountainFar;
	Sprite mountains;
	Sprite treesFar;
	Sprite treesClose;
};