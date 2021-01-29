#include "precomp.h"
#pragma once
 
class Grid
{
public:
	Grid();
	Grid(vec2 gsize, vector<Tank> tanks);
	~Grid();

	void CreateGrid(vec2 gsize);
	void CheckTanksTiles();
	vector<Tile*> GetSurroundedTiles(int tileIndex);
	vec2 GetGsize() { return gsize; }
	vector<Tile*> getTiles() { return tiles; }
	int TileIndex(vec2 pos);
private:
	vec2 gsize = NULL;
	vector<Tile*> tiles;
	vector<Tank> tanks;
};

