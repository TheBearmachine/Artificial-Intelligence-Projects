#include "Tile.h"

Tile::Tile()
{

}

Tile::~Tile()
{

}

void Tile::setNeighbor(Tile * neighbor, int index)
{
	if (index > 3 || index < 0) return;
	mNeighborNodes[index] = neighbor;
}

Tile ** Tile::getNeighbors()
{
	return mNeighborNodes;
}

void Tile::setTreeParent(Tile * parent)
{
	mTreeParent = parent;
}

Tile * Tile::getTreeParent()
{
	return mTreeParent;
}

void Tile::setAccumulatedCost(unsigned int cost)
{
	mAccumulatedCost = cost;
}

unsigned int Tile::getAccumulatedCost() const
{
	return mAccumulatedCost;
}

Tile::TileTypes Tile::getTileType() const
{
	return mTileType;
}

void Tile::setTileType(TileTypes tileType)
{
	mTileType = tileType;
}

void Tile::setAssociatedVertex(unsigned int vertex)
{
	mAssociatedVertex = vertex;
}

unsigned int Tile::getAssociatedVertex() const
{
	return mAssociatedVertex;
}

void Tile::setTileIndex(sf::Vector2u & index)
{
	mTileIndex = index;
}

sf::Vector2u Tile::getTileIndex() const
{
	return mTileIndex;
}
