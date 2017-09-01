#include "Tile.h"

Tile::Tile()
{
}

Tile::~Tile()
{
}

void Tile::setCosts(int * costs)
{
	for (int i = 0; i < MOVEMENT_TYPES_NR; i++)
	{
		mMovementCost[i] = costs[i];
	}
}

int Tile::getCost(MovementTypes movementType) const
{
	return mMovementCost[(int)movementType];
}
