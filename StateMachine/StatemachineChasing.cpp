#include "StatemachineChasing.h"
#include "StateEntityNonPlayable.h"

StatemachineChasing::StatemachineChasing(StateEntityNonPlayable * owner) :
	Statemachine(owner)
{

}

void StatemachineChasing::enter()
{
	mChaseTimer = mOwner->CHASE_TIME_MAX;
}

void StatemachineChasing::exit()
{

}

void StatemachineChasing::update(const sf::Time & deltaTime)
{
	sf::Vector2f newPos = mOwner->mLastKnownPlayerPosition - mOwner->getPosition();
	mOwner->updateSightIndicator(newPos);
	mOwner->moveInDirection(mOwner->mLastKnownPlayerPosition, deltaTime.asSeconds());

	//mOwner->mIndicator.setString("?");

	mChaseTimer -= deltaTime.asSeconds();
	if (mChaseTimer <= 0.0f)
	{
		sf::Vector2f pos = mOwner->getPosition();
		if (!mOwner->mPatrolPoints.empty())
			pos = mOwner->mPatrolPoints.front();

		startMovement(pos);
	}
}

void StatemachineChasing::startMovement(const sf::Vector2f & position)
{
	mOwner->mStatemachineMoving.setTargetPosition(position);
	transition(this, &mOwner->mStatemachineMoving);
}

void StatemachineChasing::startChasing()
{
	mChaseTimer = mOwner->CHASE_TIME_MAX;
}
