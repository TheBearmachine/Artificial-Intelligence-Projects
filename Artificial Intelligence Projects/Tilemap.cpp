#include "Tilemap.h"
#include "Tile.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <vector>

Tilemap::Tilemap() :
	mNrTilesX(0), mNrTilesY(0)
{

}

Tilemap::~Tilemap()
{

}

void Tilemap::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	states.texture = &mTileset;
	target.draw(mVertices, states);
}

void Tilemap::update(sf::Time & deltaTime)
{

}

bool Tilemap::load(const std::string & tileset, sf::Vector2u tileSize, const int * tiles, unsigned int width, unsigned int height)
{
	if (!mTileset.loadFromFile(tileset))
		return false;


	for (unsigned int i = 0; i < mNrTilesX; i++)
	{
		for (unsigned int j = 0; j < mNrTilesY; j++)
		{
			unsigned int index = i + j * mNrTilesX;
			if (mTiles[index] != nullptr)
				delete mTiles[index];
		}
	}
	delete[mNrTilesX * mNrTilesY] mTiles;

	mTiles = new Tile*[width*height];

	mVertices.setPrimitiveType(sf::Quads);
	mVertices.resize(width * height * 4);
	unsigned int halfTileWidth = tileSize.x / 2;
	unsigned int halfTileHeight = tileSize.y / 2;
	mNrTilesX = width;
	mNrTilesY = height;

	for (unsigned int i = 0; i < width; ++i)
	{
		for (unsigned int j = 0; j < height; ++j)
		{
			unsigned int index = i + j * width;
			Tile* newTile = new Tile;
			mTiles[index] = newTile;

			// get the current tile number
			int tileNumber = tiles[index];
			newTile->setTileType((Tile::TileTypes)tileNumber);
			newTile->setTileIndex(sf::Vector2u(i, j));

			// find its position in the tileset texture
			int tu = tileNumber % (mTileset.getSize().x / tileSize.x);
			int tv = tileNumber / (mTileset.getSize().x / tileSize.x);

			// get a pointer to the current tile's quad
			sf::Vertex* quad = &mVertices[index * 4];

			// define its 4 corners
			quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
			quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
			quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
			quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

			// define its 4 texture coordinates
			quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
			quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
			quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
			quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
		}
	}

	// Setup a graph
	for (unsigned int i = 0; i < width; ++i)
	{
		for (unsigned int j = 0; j < height; ++j)
		{
			unsigned int index = i + j * width;

			// Assign neighbors to each tile, except for tiles on the edge of the screen
			mTiles[index]->setNeighbor(j == 0 ? nullptr : mTiles[index - width], 0);
			mTiles[index]->setNeighbor(i == width ? nullptr : mTiles[index + 1], 1);
			mTiles[index]->setNeighbor(j == height ? nullptr : mTiles[index + width], 2);
			mTiles[index]->setNeighbor(i == 0 ? nullptr : mTiles[index - 1], 3);
			mTiles[index]->setTreeParent(nullptr);
		}
	}

	return true;
}

void Tilemap::calculatePaths(const sf::Vector2f &startPos, int travelLength, int* tileCosts)
{
	Tile* startTile;
	std::vector<Tile*> closedList;
	std::vector<Tile*> openList;
	while (!openList.empty())
	{

	}
}
