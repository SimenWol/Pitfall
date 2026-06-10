#pragma once

class Tilemap
{
public:
	Tilemap();
public:
	enum class TileTypes
	{
		Empty,
		Barrier,
		FloorYellow,
		HalfFloorYellow,
		FloorBrown,
		LadderTop,
		Ladder,
		Bricks,
		Black,
		Hole,
	};
	enum class TileContents
	{
		Empty,
		Obstacle,
		ObstacleBottomHalf,
		Ladder,
	};
public:
	void LoadMap();
	void Draw(Surface* screen, const float2 offset);
	void DrawTop(Surface* screen, const float2 offset);
	TileContents GetContents(const int2& tileLoc) const;
private:
	void DrawTile(Surface* screen, const int2& loc, const int2& tileLoc);
	void DrawHalfTile(Surface* screen, const int2& loc, const int2& tileLoc, const bool isTop);
	TileTypes GetTileType(const int num) const;
	int2 GetAtlasLoc(const TileTypes tile) const;
private:
	int2 topTileLocs[50];
private:
	// Map Loading
	string rawMapContents;
	TileTypes mapContents[50][100];

	// Sprites
	Surface tileAtlas;
public:
	static constexpr int TILESIZE = 64;
	static constexpr int MAPWIDTH = 40;
	static constexpr int MAPHEIGHT = 12;
};