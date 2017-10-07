#include "StateEntityPlayable.h"
#include "VectorFunctions.h"
#include <SFML/Window/Keyboard.hpp>

static const float RADIUS = 15.0f;
static const float THICKNESS = 3.0f;

static const float MOVEMENT_SPEED = 120.0f;

StateEntityPlayable::StateEntityPlayable()
{
	mSprite.setRadius(RADIUS);
	mSprite.setOutlineThickness(THICKNESS);
	mSprite.setOrigin(sf::Vector2f(RADIUS + THICKNESS, RADIUS + THICKNESS));
	mSprite.setFillColor(sf::Color::Green);
	mSprite.setOutlineColor(sf::Color::Black);
}

StateEntityPlayable::~StateEntityPlayable()
{

}

void StateEntityPlayable::update(const sf::Time & deltaTime)
{
	sf::Vector2f newPos(0.0f, 0.0f);
	//mStatemachine->update(deltaTime);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		newPos.x = -1;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		newPos.x = 1;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		newPos.y = -1;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		newPos.y = 1;
	}
	if (VectorFunctions::vectorMagnitude(newPos) >= 0.01)
	{
		VectorFunctions::normalizeVector(newPos);
		newPos *= (deltaTime.asSeconds() * MOVEMENT_SPEED);
		move(newPos);
	}
}
