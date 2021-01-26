#include "precomp.h"

#pragma once
class Tile
{
	public:
		Tile();
		Tile(vec2 position);
		~Tile();

		vector<Tank*> GetTanks();
		void AddToTanks(Tank* t);
		void RemoveFromTanks(Tank* t);
		vec2 GetPosition() { return position; }
	private:
		vector<Tank*> tanks;
		vec2 position;
};
