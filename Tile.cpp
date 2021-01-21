#include "precomp.h"
#include "Tile.h"

Tile::Tile()
{
}

Tile::Tile(vec2 position)
{
	this->position = position;
}

Tile::~Tile()
{
}

vector<Tank> Tile::GetTanks()
{
	return tanks;
}

void Tile::AddToTanks(Tank t)
{
	tanks.push_back(t);
	/*if(position.x == 0 || position.y == 0) 
		t.setCurrentTileIndex(position.x + position.y);
	else
		t.setCurrentTileIndex(position.x * position.y);*/
}

void Tile::RemoveFromTanks(Tank t)
{ 
	int desIndex = 0;

	for (int i = 0; i < (int)tanks.size(); i++) {
		if (&t == &tanks[i]) {
			desIndex = i;
			break;
		}
		
	}
	tanks.erase(tanks.begin() + desIndex);
}
