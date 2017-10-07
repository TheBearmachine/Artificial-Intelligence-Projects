#include "StatemachineMoving.h"
#include "StateEntityNonPlayable.h"

StatemachineMoving::StatemachineMoving(StateEntityNonPlayable * owner) :
	Statemachine(owner)
{

}

void StatemachineMoving::enter()
{
}

void StatemachineMoving::exit()
{
}

void StatemachineMoving::update(const sf::Time & deltaTime)
{
	sf::Vector2f nextPos;
	mOwner->move(nextPos);
}

void StatemachineMoving::startIdle(const float & idleTime)
{

}

void StatemachineMoving::startChasing()
{
}
