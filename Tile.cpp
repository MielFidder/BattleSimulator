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

vector<Tank*> Tile::GetTanks()
{
	return tanks;
}

void Tile::AddTankToTile(Tank* t)
{
	tanks.push_back(t);
}

void Tile::RemoveTankFromTile(Tank* t)
{ 
	for (int i = 0; i < (int)tanks.size(); i++) {
		if (t == tanks[i]) {
			tanks.erase(tanks.begin() + i);
			break;
		}
	}
}
