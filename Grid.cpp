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
		for (int y = 0; y < gsize.y; y++) {
			Tile* t = new Tile(vec2(i, y));
			tiles.push_back(t);
		}
	}
}

void Grid::CheckTanksTiles()
{
	float tsizeX = SCRWIDTH / gsize.x;
	float tsizeY = SCRHEIGHT / gsize.y;

	int indexX;
	int indexY;

	for (int i = 0; i < tiles.size(); i++) {
		for (int z = 0; z < tiles[i]->GetTanks().size(); i++) {
			indexX = floor(tanks[i].position.x / tsizeX);
			indexY = floor(tanks[i].position.y / tsizeY);
			if (tiles[i]->GetPosition().x != indexX || tiles[i]->GetPosition().y != indexY) {
				if (tanks[i].position.x > SCRWIDTH) 
					indexX = gsize.x;
				
				if (tanks[i].position.y > SCRHEIGHT) 
					indexY = gsize.y;

				int index = indexX * indexY;
				if (index > 0)
					index--;

				Tank t = tiles[i]->GetTanks()[z];

				tiles[index]->AddToTanks(t);
				tiles[i]->RemoveFromTanks(t);
			}
		}
	}
}
