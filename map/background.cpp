#include "precomp.h"
#include "map/background.h"

Background::Background()
	:background("assets/Parallax/parallax-mountain-bg.png")
	,mountainFar(new Surface("assets/Parallax/parallax-mountain-montain-far.png"), 1)
	,mountains(new Surface("assets/Parallax/parallax-mountain-mountains.png"), 1)
	,treesFar(new Surface("assets/Parallax/parallax-mountain-trees.png"), 1)
	,treesClose(new Surface("assets/Parallax/parallax-mountain-foreground-trees.png"), 1)
{}

void Background::Draw(Surface* screen, const float2 offset)
{
	// Get locations on buffers.
	uint* src = background.pixels;
	uint* dst = screen->pixels;

	// Draw static background
	for (int y = 0; y < background.height; y++, src += background.width, dst += screen->width)
	{
		for (int x = 0; x < background.width; x++)
		{
			dst[x] = src[x];
		}
	}
	
	// Draw non-static layers
	mountainFar.Draw(screen, 0 - static_cast<int>(offset.x / 4), 50 - static_cast<int>(offset.y / 4));
	mountains.Draw(screen, 0 - static_cast<int>(offset.x / 3), 75 - static_cast<int>(offset.y / 3));
	treesFar.Draw(screen, 0 - static_cast<int>(offset.x / 2), 175 - static_cast<int>(offset.y / 1.5));
	treesClose.Draw(screen, 0 - static_cast<int>(offset.x / 1.5), 125 - static_cast<int>(offset.y));
}
