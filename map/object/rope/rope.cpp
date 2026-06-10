#include "precomp.h"
#include "map/object/rope/rope.h"
#include "map/tilemap.h"

Rope::Rope(const float2 loc, const float length)
	:topLoc(loc)
	,ropeLength(length)
{
	bottomLoc.x = topLoc.x;
	bottomLoc.y = topLoc.y + length;
}

void Rope::Draw(Surface* screen, const float2 offset)
{
	for (int x = -2; x <= 2; x++)
	{
		screen->Line(topLoc.x - offset.x + x, topLoc.y - offset.y, bottomLoc.x - offset.x + x, bottomLoc.y - offset.y, 0x444400);
	}
}


void Rope::Update(const float deltaTime)
{
	timeElapsed += deltaTime;
	float t = cos(timeElapsed * swingSpeed) * swingStrength;

	bottomLoc.x = sin(t) * ropeLength + topLoc.x;
	bottomLoc.y = cos(t) * ropeLength + topLoc.y;
}

float2 Rope::GetTopLoc() const { return topLoc; }

float2 Rope::GetBottomLoc() const { return bottomLoc; }

void Rope::Reset()
{
	timeElapsed = 0.0f;
	bottomLoc.x = topLoc.x;
	bottomLoc.y = topLoc.y + ropeLength;
}
