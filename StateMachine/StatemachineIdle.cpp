#include "StatemachineIdle.h"
#include "StateEntityNonPlayable.h"
#include "VectorRotation.h"

StatemachineIdle::StatemachineIdle(StateEntityNonPlayable * owner) :
	Statemachine(owner), mIdleTimer(0.0f)
{
	mIdleTimer = mOwner->IDLE_TIME_MAX;
}

void StatemachineIdle::enter()
{
	mInitialLookDir = mOwner->mLookVector;
	mOwner->setIndicator("...");
}

void StatemachineIdle::exit()
{

}

void StatemachineIdle::update(const sf::Time & deltaTime)
{
	mIdleTimer -= deltaTime.asSeconds();
	float fancyThing = sinf((mIdleTimer / mOwner->IDLE_TIME_MAX) * 3 * PI) * 25;
	VectorRotation rot(VectorRotation::inDegrees(fancyThing));
	rot = (VectorRotation(mInitialLookDir) * rot).toVector();
	mOwner->updateSightIndicator(rot.toVector());

	if (mIdleTimer <= 0.0f)
	{
		if (!mOwner->mPatrolPoints.empty())
		{
			sf::Vector2f temp = mOwner->mPatrolPoints.front();
			mOwner->mPatrolPoints.pop();
			mOwner->mPatrolPoints.push(temp);
			startMovement(mOwner->mPatrolPoints.front());
		}
		else
		{
			mIdleTimer = mOwner->IDLE_TIME_MAX;
		}
	}
}

void StatemachineIdle::startMovement(const sf::Vector2f & position)
{
	mOwner->mStatemachineMoving.setTargetPosition(position);
	transition(this, &mOwner->mStatemachineMoving);
}

void StatemachineIdle::startChasing()
{
	transition(this, &mOwner->mStatemachineChasing);
}

void StatemachineIdle::setIdleTimer(const float & timer)
{
	mIdleTimer = timer;
}
