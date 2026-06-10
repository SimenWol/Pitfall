#include "precomp.h"
#include "camera.h"
#include "map/tilemap.h"

float2 Camera::GetOffSet() const { return offset; }

void Camera::Reset()
{
	offset.x = 0.0f;
	offset.y = 0.0f;
}

void Camera::Tick(const Surface* const screen, const float2 playerLoc, const float deltaTime)
{
	if (offset.x != 0 || playerLoc.x > screen->width / 2.0f)
	{
		offset.x = offset.x * (1 - deltaTime * 5.0f) + (playerLoc.x - screen->width / 2.0f) * deltaTime * 5.0f; // Got this line from Remi
		if (offset.x < 0) { offset.x = 0; }
		if (offset.x > Tilemap::MAPWIDTH * Tilemap::TILESIZE - screen->width)
		{
			offset.x = static_cast<float>(Tilemap::MAPWIDTH * Tilemap::TILESIZE - screen->width);
		}
	}

	if (offset.y != 0 || playerLoc.y > screen->height / 2.0f)
	{
		offset.y = offset.y * (1 - deltaTime * 2.5f) + (playerLoc.y - screen->height / 2.0f) * deltaTime * 2.5f; // Got this line from Remi
		if (offset.y < 0) { offset.y = 0; }
		if (offset.y > Tilemap::MAPHEIGHT * Tilemap::TILESIZE - screen->height)
		{
			offset.y = static_cast<float>(Tilemap::MAPHEIGHT * Tilemap::TILESIZE - screen->height);
		}
	}
}