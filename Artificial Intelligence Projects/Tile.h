#pragma once
#include <SFML/System/Vector2.hpp>

class Tile
{
public:

	static const int TILE_TYPES_NR = 6;
	enum TileTypes
	{
		Forrest,
		Mountains,
		ShallowWater,
		DeepWater,
		Plains,
		Bridge
	};

	Tile();
	~Tile();

	// North neighbor is index 0 and goes clockwise
	void setNeighbor(Tile* neighbor, int index);
	Tile** getNeighbors();

	void setTreeParent(Tile* parent);
	Tile* getTreeParent();

	TileTypes getTileType() const;
	void setTileType(TileTypes tileType);

	void setAssociatedVertex(unsigned int vertex);
	unsigned int getAssociatedVertex() const;

	void setTileIndex(sf::Vector2u &index);
	sf::Vector2u getTileIndex() const;

private:
	Tile* mNeighborNodes[4];
	Tile* mTreeParent;
	TileTypes mTileType;
	unsigned int mAssociatedVertex;
	sf::Vector2u mTileIndex;
};