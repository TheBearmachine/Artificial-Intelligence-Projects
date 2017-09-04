#pragma once
#include "Tilemap.h"
#include "Entity.h"
#include <SFML/Graphics/RenderWindow.hpp>

class TimemapProgram
{
public:
	TimemapProgram();
	~TimemapProgram();

	void run();

private:
	void update(sf::Time &deltaTime);
	void draw(sf::RenderWindow& window);

	Tilemap mTilemap;
	Entity mEntities[4];
	Entity* mSelectedEntity;
};