#include "StatemachineProgram.h"
#include <SFML/Window/Event.hpp>

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

#pragma region patrolInit
	std::vector<sf::Vector2f> patVec0;
	patVec0.push_back(sf::Vector2f(100, 100));
	patVec0.push_back(sf::Vector2f(100, 500));

	std::vector<sf::Vector2f> patVec1;
	patVec1.push_back(sf::Vector2f(400, 400));
	patVec1.push_back(sf::Vector2f(200, 400));
	patVec1.push_back(sf::Vector2f(200, 200));
	patVec1.push_back(sf::Vector2f(400, 200));

	std::vector<sf::Vector2f> patVec2;
	patVec2.push_back(sf::Vector2f(500, 300));
	patVec2.push_back(sf::Vector2f(100, 400));

	std::vector<sf::Vector2f> patVec3;
	patVec3.push_back(sf::Vector2f(300, 100));
	patVec3.push_back(sf::Vector2f(100, 300));
	patVec3.push_back(sf::Vector2f(200, 200));
	patVec3.push_back(sf::Vector2f(200, 100));
#pragma endregion

	mPlayerEntity.setPosition(320, 240);
	mTestEntities[0].setPosition(200, 200);
	mTestEntities[0].setPlayerReference(&mPlayerEntity);
	mTestEntities[0].initalizePatrolQueue(patVec0);
	mTestEntities[1].setPosition(400, 200);
	mTestEntities[1].setPlayerReference(&mPlayerEntity);
	mTestEntities[1].initalizePatrolQueue(patVec1);
	mTestEntities[2].setPosition(200, 400);
	mTestEntities[2].setPlayerReference(&mPlayerEntity);
	mTestEntities[2].initalizePatrolQueue(patVec2);
	mTestEntities[3].setPosition(400, 400);
	mTestEntities[3].setPlayerReference(&mPlayerEntity);
	mTestEntities[3].initalizePatrolQueue(patVec3);

	while (mWindow.isOpen())
	{
		sf::Time time = clock.restart();
		sf::Event event;
		while (mWindow.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				mWindow.close();

			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
			{
				mWindow.close();
			}
		}
		update(time);
		draw();
	}
}

void StatemachineProgram::update(const sf::Time & deltaTime)
{
	for (size_t i = 0; i < NR_ENTITIES; i++)
		mTestEntities[i].update(deltaTime);
	mPlayerEntity.update(deltaTime);
}

void StatemachineProgram::draw()
{
	mWindow.clear(sf::Color(155, 155, 155));
	for (size_t i = 0; i < NR_ENTITIES; i++)
		mWindow.draw(mTestEntities[i]);
	mWindow.draw(mPlayerEntity);
	mWindow.display();
}
