#include "StatemachineProgram.h"

StatemachineProgram::StatemachineProgram()
{

}

StatemachineProgram::~StatemachineProgram()
{

}

void StatemachineProgram::run()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	mWindow.create(sf::VideoMode(640, 480), "Statemachine Program", 7U, settings);
	sf::Clock clock;
	clock.restart();

	mTestEntities[0].setPosition(200, 200);
	mTestEntities[1].setPosition(400, 200);
	mTestEntities[2].setPosition(200, 400);
	mTestEntities[3].setPosition(400, 400);

	while (mWindow.isOpen())
	{
		sf::Time time = clock.restart();
		update(time);
		draw();
	}
}

void StatemachineProgram::update(const sf::Time & deltaTime)
{
	for (size_t i = 0; i < NR_ENTITIES; i++)
		mTestEntities[i].update(deltaTime);
}

void StatemachineProgram::draw()
{
	mWindow.clear(sf::Color(155, 155, 155));
	for (size_t i = 0; i < NR_ENTITIES; i++)
		mWindow.draw(mTestEntities[i]);
	mWindow.display();
}
