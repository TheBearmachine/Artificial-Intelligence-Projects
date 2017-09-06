#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Time.hpp>
#include <vector>

class Tile;

class Tilemap : public sf::Drawable, public sf::Transformable
{
public:
	Tilemap();
	~Tilemap();

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void update(sf::Time &deltaTime);

	bool load(const std::string& tileset, sf::Vector2u tileSize, const int* tiles, unsigned int width, unsigned int height);

	void calculatePaths(const sf::Vector2f &startPos, int travelLength, int* tileCosts);
	void clearPaths();

private:
	unsigned int getIndexFromVector(const sf::Vector2f &pos);

	sf::VertexArray mVertices;
	sf::Texture mTileset;
	Tile** mTiles;
	std::vector<Tile*> mAvaliableMoves;
	float mMovesTimer;

	unsigned int mNrTilesX;
	unsigned int mNrTilesY;
	unsigned int mTileWidth;
	unsigned int mTileHeight;
};