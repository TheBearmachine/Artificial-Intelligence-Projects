#include "StateEntityNonPlayable.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include "VectorRotation.h"

static const float FIELD_OF_VIEW = 45.0f;
static const float SIGHT_DISTANCE = 120.0f;

static const float ROTATION_SPEED = 1.0f;

static const float RADIUS = 15.0f;
static const float THICKNESS = 3.0f;

StateEntityNonPlayable::StateEntityNonPlayable() :
	mSearchCone(sf::PrimitiveType::TriangleFan, 6),
	time(0.0f),
	mStatemachineIdle(this),
	mCurrentState(&mStatemachineIdle)
{
	mSprite.setRadius(RADIUS);
	mSprite.setOutlineThickness(THICKNESS);
	mSprite.setOrigin(sf::Vector2f(RADIUS + THICKNESS, RADIUS + THICKNESS));
	mSprite.setFillColor(sf::Color::Blue);
	mSprite.setOutlineColor(sf::Color::Black);

	mLookVector = sf::Vector2f(0.0f, -1.0f);
	// Initialize searchcone positions
	/*VectorRotation rot1(VectorRotation::inDegrees(FIELD_OF_VIEW / 2.0f));
	VectorRotation rot2(VectorRotation::inDegrees(-FIELD_OF_VIEW / (float)(mSearchCone.getVertexCount() - 2)));

	mSearchCone[0].position = sf::Vector2f(0.0f, 0.0f);*/
	mSearchCone[0].color = sf::Color(255, 0, 0, 125);
	//sf::Vector2f newPos(0.0f, SIGHT_DISTANCE);
	//newPos = (VectorRotation(newPos) * rot1).toVector();
	for (size_t i = 1; i < mSearchCone.getVertexCount(); i++)
	{
		//mSearchCone[i].position = newPos;
		mSearchCone[i].color = sf::Color(255, 255, 0, 125);
		//newPos = (VectorRotation(newPos) * rot2).toVector();
	}
}

StateEntityNonPlayable::~StateEntityNonPlayable()
{

}

void StateEntityNonPlayable::update(const sf::Time & deltaTime)
{
	time += deltaTime.asSeconds();
	mLookVector.x = sinf(time);
	VectorRotation rotier(mLookVector);
	rotier.normalize();
	mLookVector = rotier.toVector();

	sf::Vector2f forwardPos = mLookVector * SIGHT_DISTANCE;
	VectorRotation rot1(VectorRotation::inDegrees(FIELD_OF_VIEW / 2.0f));
	VectorRotation rot2(VectorRotation::inDegrees(-FIELD_OF_VIEW / (float)(mSearchCone.getVertexCount() - 2)));
	forwardPos = (VectorRotation(forwardPos) * rot1).toVector();

	for (size_t i = 1; i < mSearchCone.getVertexCount(); i++)
	{
		mSearchCone[i].position = forwardPos;
		forwardPos = (VectorRotation(forwardPos) * rot2).toVector();
	}

	mCurrentState->update(deltaTime);
}

void StateEntityNonPlayable::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(mSearchCone, states);
	target.draw(mSprite, states);
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
