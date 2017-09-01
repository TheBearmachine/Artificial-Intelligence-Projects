#pragma once

class Tile
{
public:

	static const int MOVEMENT_TYPES_NR = 4;
	enum MovementTypes
	{
		Legs,
		Wings,
		Fins,
		Wheels
	};

	Tile();
	~Tile();

	void setCosts(int* costs);
	int getCost(MovementTypes movementType)const;

private:
	int mMovementCost[MOVEMENT_TYPES_NR];
};