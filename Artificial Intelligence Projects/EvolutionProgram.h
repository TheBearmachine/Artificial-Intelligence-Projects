#pragma once
#include "Entity.h"
#include "Tilemap.h"
#include <SFML/Graphics/RenderWindow.hpp>

class EvolutionProgram : public IEntityListener
{
public:
	EvolutionProgram();
	~EvolutionProgram();

	void run();

private:
	void update(sf::Time &deltaTime);
	void draw(sf::RenderWindow& window);

	virtual void destinationReached() override;

	Tilemap mTilemap;
	Entity mEntity;
	bool mEntityInTransit;
};