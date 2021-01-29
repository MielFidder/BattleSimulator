#include "precomp.h"
#pragma once
 
class Grid
{
public:
	Grid();
	Grid(vec2 gsize, vector<Tank> tanks);
	~Grid();

	void CreateGrid(vec2 gsize);
	void UpdateTanksInTiles();
	vector<Tile*> GetSurroundedTiles(int tileIndex);
	vec2 GetGsize() { return gsize; }
	vector<Tile*> getTiles() { return tiles; }
	int GetTileIndex(vec2 pos);
private:
	vec2 gsize = NULL;
	vector<Tile*> tiles;
	vector<Tank> tanks;
};

