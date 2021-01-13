#include "precomp.h"

#pragma once
class Tile
{
public:
	Tile();
	~Tile();

	vector<Tank*> GetTanks();
	void AddToTanks(Tank* t);
	void RemoveFromTanks(Tank* t);
private:
	vector<Tank*> tanks;
};
