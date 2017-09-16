#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include "Tilemap.h"

class TilemapGenerator
{
public:
	TilemapGenerator();
	~TilemapGenerator();

	void run(size_t nrTilesX, size_t nrTilesY, size_t defaultTile = 0);

private:
	void update(sf::Time &deltaTime);
	void draw(sf::RenderWindow& window);

	Tilemap mTilemap;
	size_t mPaintColor;
	int *mTileIndexArray;
	bool mMouseButtonDown;
};