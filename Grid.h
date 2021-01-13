#include "precomp.h"
#pragma once

class Grid
{
public:
	Grid();
	Grid(vec2 gsize, vector<Tank*> tanks);
	~Grid();

	void CreateGrid(vec2 gsize);
private:
	void UpdateTankTiles();

	vec2 gsize = NULL;
	vector<Tile*> tiles;
	vector<Tank*> tanks;
};

