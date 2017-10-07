#include "StatemachineMoving.h"
#include "StateEntityNonPlayable.h"

StatemachineMoving::StatemachineMoving(StateEntityNonPlayable * owner) :
	Statemachine(owner)
{

}

void StatemachineMoving::enter()
{
	mOwner->updateSightIndicator(mTargetPosition - mOwner->getPosition());
	mOwner->setIndicator("<-->");
}

void StatemachineMoving::exit()
{

}

void StatemachineMoving::update(const sf::Time & deltaTime)
{
	sf::Vector2f newPos = mOwner->mPatrolPoints.front();
	if (mOwner->moveInDirection(newPos, deltaTime.asSeconds()))
	{
		startIdle(mOwner->CHASE_TIME_MAX);
	}
}

void StatemachineMoving::startIdle(const float & idleTime)
{
	mOwner->mStatemachineIdle.setIdleTimer(idleTime);
	transition(this, &mOwner->mStatemachineIdle);
}

void StatemachineMoving::startChasing()
{
	transition(this, &mOwner->mStatemachineChasing);
}

void StatemachineMoving::setTargetPosition(const sf::Vector2f &tarPos)
{
	mTargetPosition = tarPos;
}
