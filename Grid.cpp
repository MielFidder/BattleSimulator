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

				tiles[i]->GetTanks()[z].setCurrentTileIndex(index); // set tile index in tank

				tiles[index]->AddToTanks(tiles[i]->GetTanks[z]); // remove tank from old tile
				tiles[i]->RemoveFromTanks(tiles[i]->GetTanks[z]); // add tank to new tile
			}
		}
	}
}

vector<Tile*> Grid::GetSurroundedTiles(int tileIndex) {
	vector<Tile*> surroundingTiles;
	Tile* original = tiles.at(tileIndex);

	if (original->GetPosition().y > (GRIDROW -1)) {
		Tile* up = tiles.at(tileIndex - static_cast<__int64>(GRIDROW));
		surroundingTiles.push_back(up);
	}

	if (original->GetPosition().x > 0) {
		Tile* left = tiles.at(tileIndex - static_cast<__int64>(1));
		surroundingTiles.push_back(left);
	}

	if (original->GetPosition().x < GRIDROW) {
		Tile* right = tiles.at(tileIndex + static_cast<__int64>(1));
		surroundingTiles.push_back(right);
	}

	if (original->GetPosition().y < ((GRIDROW * GRIDCOL) - GRIDROW) -1) {
		Tile* down = tiles.at(tileIndex + static_cast<__int64>(GRIDROW));
		surroundingTiles.push_back(down);
	}

	surroundingTiles.push_back(original);

	return surroundingTiles;
}
