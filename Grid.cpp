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
	//get size of tile
	float tsizeX = SCRWIDTH / gsize.x; 
	float tsizeY = SCRHEIGHT / gsize.y;

	int indexX;
	int indexY;

	for (int i = 0; i < tiles.size(); i++) { //loop through all tiles
		for (int z = 0; z < tiles[i]->GetTanks().size(); i++) { //loop though all tanks in tiles
			indexX = floor(tanks[i].position.x / tsizeX); // check what tile tank is on x axis
			indexY = floor(tanks[i].position.y / tsizeY); // check what tile tank is on y axis
			if (tiles[i]->GetPosition().x != indexX || tiles[i]->GetPosition().y != indexY) { // if one of the conditions change
				if (tanks[i].position.x > SCRWIDTH) 
					indexX = gsize.x;
				
				if (tanks[i].position.y > SCRHEIGHT) 
					indexY = gsize.y;

				int index = indexX * indexY;
				if (index > 0) 
					index--;

				tiles[index]->AddToTanks(tiles[i]->GetTanks[z]); // add tank to new tile
				tiles[i]->RemoveFromTanks(tiles[i]->GetTanks[z]); // remove tank from old tile
			}
		}
	}
}
