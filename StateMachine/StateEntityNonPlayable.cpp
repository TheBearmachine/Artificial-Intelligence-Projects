#include "StateEntityNonPlayable.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include "VectorFunctions.h"
#include "StateEntityPlayable.h"

static const float FIELD_OF_VIEW = 45.0f;
static const float HALF_FOV = FIELD_OF_VIEW / 2.0f;
static const float SIGHT_DISTANCE = 120.0f;

static const float MOVEMENT_SPEED_AI = 80.0f;

static const char* FONT = "../sui_generis_rg.ttf";

static const float RADIUS = 15.0f;
static const float THICKNESS = 3.0f;

StateEntityNonPlayable::StateEntityNonPlayable() :
	mSearchCone(sf::PrimitiveType::TriangleFan, 6),
	time(0.0f),
	mPlayer(nullptr),
	mStatemachineIdle(this),
	mStatemachineMoving(this),
	mStatemachineChasing(this),
	mCurrentState(&mStatemachineIdle)
{
	mStatemachineIdle.setIdleTimer(0.0f);

	mIndicatorFont.loadFromFile(FONT);
	mIndicator.setFont(mIndicatorFont);
	mIndicator.setCharacterSize(36);
	mIndicator.setFillColor(sf::Color::White);
	mIndicator.setOutlineColor(sf::Color::Black);
	mIndicator.setOutlineThickness(3.0f);
	mIndicator.setPosition(0.0f, -RADIUS - THICKNESS - (float)mIndicator.getCharacterSize());
	setIndicator("...");

	mSprite.setRadius(RADIUS);
	mSprite.setOutlineThickness(THICKNESS);
	mSprite.setOrigin(sf::Vector2f(RADIUS + THICKNESS, RADIUS + THICKNESS));
	mSprite.setFillColor(sf::Color::Red);
	mSprite.setOutlineColor(sf::Color::Black);

	mLookVector = sf::Vector2f(0.0f, -1.0f);
	mSearchCone[0].color = sf::Color(255, 0, 0, 125);

	for (size_t i = 1; i < mSearchCone.getVertexCount(); i++)
	{
		mSearchCone[i].color = sf::Color(255, 255, 0, 125);
	}
	updateSightIndicator(mLookVector);
}

StateEntityNonPlayable::~StateEntityNonPlayable()
{

}

void StateEntityNonPlayable::setPlayerReference(StateEntityPlayable * player)
{
	mPlayer = player;
}

void StateEntityNonPlayable::update(const sf::Time & deltaTime)
{
	
	mCurrentState->update(deltaTime);

	// Look to see if the player is within the field of view
	if (mPlayer)
	{
		sf::Vector2f tarDir = mPlayer->getPosition() - getPosition();
		if (VectorFunctions::vectorMagnitude(tarDir) < SIGHT_DISTANCE)
		{
			float angle = VectorFunctions::angleBetweenVectors(tarDir, mLookVector);
			angle = angle * 180 / PI;
			if (angle < HALF_FOV)
			{
				// Player found, get him (Or her, #feminism)!
				mLastKnownPlayerPosition = mPlayer->getPosition();
				setIndicator("!");
				mCurrentState->startChasing();
			}
		}
	}
}

void StateEntityNonPlayable::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(mSearchCone, states);
	target.draw(mSprite, states);
	target.draw(mIndicator, states);
}

bool StateEntityNonPlayable::moveInDirection(const sf::Vector2f & direction, const float & deltaTime)
{
	bool goalReached = false;
	sf::Vector2f newPos = VectorFunctions::lerp(getPosition(), direction, deltaTime * MOVEMENT_SPEED_AI, goalReached);
	setPosition(newPos);
	return goalReached;
}

void StateEntityNonPlayable::updateSightIndicator(const sf::Vector2f &direction)
{
	if (VectorFunctions::vectorMagnitude(direction) >= 0.01)
	{
		mLookVector = direction;
		VectorFunctions::normalizeVector(mLookVector);
	}

	sf::Vector2f forwardPos = mLookVector * SIGHT_DISTANCE;
	VectorRotation rot1(VectorRotation::inDegrees(FIELD_OF_VIEW / 2.0f));
	VectorRotation rot2(VectorRotation::inDegrees(-FIELD_OF_VIEW / (float)(mSearchCone.getVertexCount() - 2)));
	forwardPos = (VectorRotation(forwardPos) * rot1).toVector();

	for (size_t i = 1; i < mSearchCone.getVertexCount(); i++)
	{
		mSearchCone[i].position = forwardPos;
		forwardPos = (VectorRotation(forwardPos) * rot2).toVector();
	}
}

void StateEntityNonPlayable::setIndicator(const std::string & str)
{
	mIndicator.setString(str);
	mIndicator.setOrigin(mIndicator.getLocalBounds().width / 2.0f, 0.0f);
}

void StateEntityNonPlayable::initalizePatrolQueue(const std::vector<sf::Vector2f> &points)
{
	while (!mPatrolPoints.empty())
		mPatrolPoints.pop();

	for (size_t i = 0; i < points.size(); i++)
	{
		mPatrolPoints.push(points[i]);
	}
}
