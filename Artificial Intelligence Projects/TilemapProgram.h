#pragma once
#include "Tilemap.h"
#include "Entity.h"
#include <SFML/Graphics/RenderWindow.hpp>

class TilemapProgram: public IEntityListener
{
public:
	TilemapProgram();
	~TilemapProgram();

	void run();

private:
	void update(sf::Time &deltaTime);
	void draw(sf::RenderWindow& window);

	void destinationReached();

	Tilemap mTilemap;
	Entity mEntities[4];
	Entity* mSelectedEntity;
	bool mEntityInTransit;
};