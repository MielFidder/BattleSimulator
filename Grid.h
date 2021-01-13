#include "precomp.h"
#pragma once

class Grid
{
public:
	Grid();
	Grid(vec2 gsize);
	~Grid();

	void CreateGrid(vec2 gsize);
private:
	vec2 gsize = NULL;
	vector<Tile*> tiles;
};

