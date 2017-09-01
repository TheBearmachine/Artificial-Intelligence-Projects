#include "Tilemap.h"
#include <SFML/Graphics/RenderWindow.hpp>

Tilemap::Tilemap()
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

bool Tilemap::load(const std::string & tileset, sf::Vector2u tileSize, const int * tiles, unsigned int width, unsigned int height)
{
	if (!mTileset.loadFromFile(tileset))
		return false;

	mVertices.setPrimitiveType(sf::Quads);
	mVertices.resize(width * height * 4);

	for (unsigned int i = 0; i < width; ++i)
		for (unsigned int j = 0; j < height; ++j)
		{
			// get the current tile number
			int tileNumber = tiles[i + j * width];

			// find its position in the tileset texture
			int tu = tileNumber % (mTileset.getSize().x / tileSize.x);
			int tv = tileNumber / (mTileset.getSize().x / tileSize.x);

			// get a pointer to the current tile's quad
			sf::Vertex* quad = &mVertices[(i + j * width) * 4];

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

	return true;
}
