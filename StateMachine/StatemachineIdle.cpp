#include "StatemachineIdle.h"
#include "StateEntityNonPlayable.h"

StatemachineIdle::StatemachineIdle(StateEntityNonPlayable * owner) :
	Statemachine(owner)
{

}

void StatemachineIdle::enter()
{

}

void StatemachineIdle::exit()
{

}

void StatemachineIdle::update(const sf::Time & deltaTime)
{

}

void StatemachineIdle::startMovement(const sf::Vector2f & position)
{

	//transition(this, /*mStatemachineMovement*/);
}

void StatemachineIdle::startChasing()
{
}
