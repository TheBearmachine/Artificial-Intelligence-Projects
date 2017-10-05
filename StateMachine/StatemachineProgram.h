#pragma once
#include "StateEntityNonPlayable.h"
#include <SFML/Graphics/RenderWindow.hpp>

class StatemachineProgram
{
public:
	StatemachineProgram();
	~StatemachineProgram();

	void run();

private:
	void update(const sf::Time &deltaTime);
	void draw();

	static const size_t NR_ENTITIES = 4;
	StateEntityNonPlayable mTestEntities[NR_ENTITIES];
	sf::RenderWindow mWindow;
};