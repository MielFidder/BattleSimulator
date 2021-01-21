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

	for (int i = 0; i < gsize.y; i++) {
		for (int y = 0; y < gsize.x; y++) {
			Tile* t = new Tile(vec2(y, i));
			tiles.push_back(t);
		}
	}
}

void Grid::CheckTanksTiles()
{
	//get size of tile
	float tsizeX = SCRWIDTH / gsize.x; 
	float tsizeY = SCRHEIGHT / gsize.y;
	
	int index;
	int indexX;
	int indexY;

	for (int i = 0; i < tiles.size(); i++) { //loop through all tiles

		for (Tank tank : tiles[i]->GetTanks()) {
			vec2 startpos = tank.getstartpos();
			indexX = (tank.position.x / tsizeX); // check what tile tank is on x axis
			indexY = (tank.position.y / tsizeY); // check what tile tank is on y axis
			if (tank.position.x > SCRWIDTH)
				indexX = (GRIDROW - 1);

			if (tank.position.y > SCRHEIGHT)
				indexY = (GRIDCOL - 1);

			if (tiles[i]->GetPosition().x != indexX || tiles[i]->GetPosition().y != indexY) { // if one of the conditions change

				index = ((GRIDROW * indexY) + indexX);

				tank.setCurrentTileIndex(index); // set tile index in tank

				tiles[index]->AddToTanks(tank); // add tank to new tile
				tiles[i]->RemoveFromTanks(tank); // remove tank from old tile
			}
		}
	}
}

vector<Tile*> Grid::GetSurroundedTiles(int tileIndex) {
	vector<Tile*> surroundingTiles;
	Tile* original = tiles.at(tileIndex);
	surroundingTiles.push_back(original);

	if (original->GetPosition().y > 0) {
		Tile* up = tiles.at(tileIndex - static_cast<__int64>(GRIDROW));
		surroundingTiles.push_back(up);
	}

	if (original->GetPosition().x > 0) {
		Tile* left = tiles.at(tileIndex - static_cast<__int64>(1));
		surroundingTiles.push_back(left);
	}

	if (original->GetPosition().x < GRIDROW -1) {
		Tile* right = tiles.at(tileIndex + static_cast<__int64>(1));
		surroundingTiles.push_back(right);
	}

	if (original->GetPosition().y < (GRIDCOL -1)) {
		Tile* down = tiles.at(tileIndex + static_cast<__int64>(GRIDROW));
		surroundingTiles.push_back(down);
	}

	return surroundingTiles;
}
