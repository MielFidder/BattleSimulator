#include "precomp.h"
#include "Grid.h"

Grid::Grid()
{
	CreateGrid(vec2(5,5));
}

Grid::Grid(vec2 gsize, vector<Tank> tanks)
{
	this->tanks = tanks;
	CreateGrid(gsize);
}

Grid::~Grid()
{
}

void Grid::CreateGrid(vec2 gsize)
{
	this->gsize = gsize;

	for (int i = 0; i < gsize.x; i++) {
		for (int y = 0; y < gsize.y; i++) {
			Tile* t = new Tile(vec2(i, y));
			tiles.push_back(t);
		}
	}
}
