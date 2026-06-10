#include "precomp.h"
#include "map/tilemap.h"

Tilemap::Tilemap()
	:tileAtlas("assets/Level/Pitfall_tilesheet.png")
{
	// Initialise local Tilemap array.
	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 50; j++)
		{
			mapContents[j][i] = TileTypes::Empty;
		}
	}

	// Initialise topTileLocs array.
	for (int i = 0; i < 50; i++)
	{
		topTileLocs[i] = { -1, -1 };
	}
}

void Tilemap::LoadMap()
{
	// Set local Tilemap array to empty.
	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 50; j++)
		{
			mapContents[j][i] = TileTypes::Empty;
		}
	}
	// Set topTileLocs array.
	for (int i = 0; i < 50; i++)
	{
		topTileLocs[i] = { -1, -1 };
	}

	// Variables required for map loading.
	int y = 0;
	int x = 0;
	int xOffset = 0;
	int tileNum = 0;
	int numTopTiles = 0;

	rawMapContents = TextFileRead("assets/Level/TileMap_Test.csv");

	// Loop through received data, get the number & write to local array.
	for (size_t i = 0; i < rawMapContents.length() - 1; i++)
	{
		if (rawMapContents[i] != '\n' && rawMapContents[i] != ',')
		{
			tileNum = (tileNum * 10) + (rawMapContents[i] - '0'); // Substract '0' due to char to int conversion through ASCII chart.
		}
		else
		{
			mapContents[y][x - xOffset] = GetTileType(tileNum);
			if (GetTileType(tileNum) == TileTypes::LadderTop || GetTileType(tileNum) == TileTypes::Hole)
			{
				topTileLocs[numTopTiles] = { x - xOffset, y };
				numTopTiles++;
			}
			tileNum = 0;
			x++;

			if (rawMapContents[i] == '\n') { y++; xOffset = x; }
		}
	}

	// Save the last tile as well.
	mapContents[y][x - xOffset] = GetTileType(tileNum);
	if (GetTileType(tileNum) == TileTypes::LadderTop || GetTileType(tileNum) == TileTypes::Hole)
	{
		topTileLocs[numTopTiles] = { x - xOffset, y };
	}
}


void Tilemap::Draw(Surface* screen, const float2 offset)
	{
		// Loop through loaded map content and calls DrawTile for each entry.
		for (int y = 0; y < MAPHEIGHT; y++)
		{
			for (int x = 0; x < MAPWIDTH; x++)
			{
				Tilemap::TileTypes tile = mapContents[y][x];
				if (tile == TileTypes::LadderTop || tile == TileTypes::Hole)
				{
					DrawHalfTile(screen, { static_cast<int>(x * TILESIZE - offset.x), static_cast<int>(y * TILESIZE - offset.y) }, GetAtlasLoc(tile), true);
				}
				else if (tile != TileTypes::Empty && tile != TileTypes::Barrier)
				{
					DrawTile(screen, { static_cast<int>(x * TILESIZE - offset.x), static_cast<int>(y * TILESIZE - offset.y) }, GetAtlasLoc(tile));
				}
			}
		}
	}

void Tilemap::DrawTop(Surface* screen, const float2 offset)
	{
		// Loop through loaded top tile content and calls DrawTile for each entry.
		for (int i = 0; i < 50; i++)
		{
			if (topTileLocs[i].y == -1) { return; }
			TileTypes tile = mapContents[topTileLocs[i].y][topTileLocs[i].x];
			switch (tile)
			{
			case Tilemap::TileTypes::LadderTop:
			case Tilemap::TileTypes::Hole:
				DrawHalfTile(screen, { static_cast<int>(topTileLocs[i].x * TILESIZE - offset.x), static_cast<int>(topTileLocs[i].y * TILESIZE - offset.y)}, GetAtlasLoc(tile), false);
				break;
			}

		}
	}

Tilemap::TileContents Tilemap::GetContents(const int2& tileLoc) const
{
	// Get tile position in 2D array.
	const int tileLocX = ((tileLoc.x - (tileLoc.x % TILESIZE)) / TILESIZE);
	const int tileLocY = ((tileLoc.y - (tileLoc.y % TILESIZE)) / TILESIZE);

	// Get tile & return its corresponding (collision) contents.
	switch (mapContents[tileLocY][tileLocX])
	{
	case TileTypes::FloorYellow:
	case TileTypes::FloorBrown:
	case TileTypes::Bricks:
		return TileContents::Obstacle;
	case TileTypes::HalfFloorYellow:
		return TileContents::ObstacleBottomHalf;
	case TileTypes::Ladder:
	case TileTypes::LadderTop:
		return TileContents::Ladder;
	default:
		return TileContents::Empty;
	}
}

void Tilemap::DrawTile(Surface* screen, const int2& loc, const int2& tileLoc)
{
	// Most of the code from this function has been adapted from the 3DGEP tutorials (https://www.3dgep.com/)

	// Checks if location is on the screen.
	if (loc.x > screen->width) { return; }
	if (loc.y > screen->height) { return; }

	// Get locations on buffers.
	uint* src = tileAtlas.pixels + tileLoc.x * TILESIZE + tileLoc.y * TILESIZE * tileAtlas.width;
	uint* dst = screen->pixels + loc.x + loc.y * screen->width;

	if (loc.x >= 0 && loc.x + TILESIZE < screen->width + 1 && loc.y >= 0 && loc.y + TILESIZE <= screen->height)
	{
		for (int loopY = 0; loopY < TILESIZE; loopY++, src += tileAtlas.width, dst += screen->width)
		{
			for (int loopX = 0; loopX < TILESIZE; loopX++)
			{
				dst[loopX] = src[loopX];
			}
		}
	}
	else	// Only do checks if tile is partially offscreen.
	{
		for (int loopY = 0; loopY < TILESIZE; loopY++, src += tileAtlas.width, dst += screen->width)
		{
			if (loc.y + loopY + 1 <= screen->height && loc.y + loopY >= 0) // Stop drawing offscreen (y).
			{
				for (int loopX = 0; loopX < TILESIZE; loopX++)
				{
					if ((loc.x + loopX + 1) <= screen->width && loc.x + loopX >= 0) { dst[loopX] = src[loopX]; } // Stop drawing offscreen (x). 
				}
			}
			else { return; }
		}
	}
}

void Tilemap::DrawHalfTile(Surface* screen, const int2& loc, const int2& tileLoc, const bool isTop) // Draws only the top or bottom half of a tile
{
	// Most of the code from this function has been adapted from the 3DGEP tutorials (https://www.3dgep.com/)

	// Checks if location is on the screen.
	if (loc.x > screen->width) { return; }
	if (loc.y > screen->height) { return; }

	// Get locations on buffers.
	uint* src = tileAtlas.pixels + tileLoc.x * TILESIZE + tileLoc.y * TILESIZE * tileAtlas.width;
	uint* dst = screen->pixels + loc.x + loc.y * screen->width;

	if (!isTop)
	{
		src += TILESIZE / 2 * tileAtlas.width;
		dst += (TILESIZE / 2) * screen->width;
	}
	if (loc.x >= 0 && loc.x + TILESIZE < screen->width + 1 && loc.y >= 0 && loc.y + TILESIZE <= screen->height)
	{
		for (int loopY = 0; loopY < TILESIZE / 2; loopY++, src += tileAtlas.width, dst += screen->width)
		{
			for (int loopX = 0; loopX < TILESIZE; loopX++)
			{
				dst[loopX] = src[loopX];
			}
		}
	}
	else
	{
		for (int loopY = 0; loopY < TILESIZE / 2; loopY++, src += tileAtlas.width, dst += screen->width)
		{
			if (loc.y + loopY + 1 <= screen->height && loc.y + loopY >= 0) // Stop drawing offscreen (y).
			{
				for (int loopX = 0; loopX < TILESIZE; loopX++)
				{
					if ((loc.x + loopX + 1) <= screen->width && loc.x + loopX >= 0) { dst[loopX] = src[loopX]; } // Stop drawing offscreen (x). 
				}
			}
			else { return; }
		}
	}
}

Tilemap::TileTypes Tilemap::GetTileType(const int num) const
{
	switch (num)
	{
	case 0:
		return TileTypes::FloorYellow;
	case 1:
		return TileTypes::FloorBrown;
	case 2:
		return TileTypes::LadderTop;
	case 5:
		return TileTypes::Ladder;
	case 6:
		return TileTypes::Bricks;
	case 7:
		return TileTypes::Black;
	case 30:
		return TileTypes::HalfFloorYellow;
	case 33:
		return TileTypes::Hole;
	default:
		return TileTypes::Empty;
	}
}

int2 Tilemap::GetAtlasLoc(const TileTypes tile) const
{
	// Get tile & return its location on the Surface.
	switch (tile)
	{
	case TileTypes::FloorYellow: 
	case TileTypes::HalfFloorYellow:
		return { 0, 0 };
	case TileTypes::FloorBrown:
		return { 1, 0 };
	case TileTypes::Hole:
	case TileTypes::LadderTop:
		return { 2, 0 };
	case TileTypes::Ladder:
		return { 3, 0 };
	case TileTypes::Bricks:
		return { 4, 0 };
	case TileTypes::Black:
		return { 5, 0 };
	default:
		return { 0,0 };
	}
}