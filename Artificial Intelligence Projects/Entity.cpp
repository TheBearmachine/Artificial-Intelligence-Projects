#include "Entity.h"
#include <SFML/Graphics/RenderWindow.hpp>

static const char* DEFAULT_TEXTURE = "DefaultImage.png";
static const float SPEED_FACTOR = 40.0f;

Entity::Entity() :
	mCosts{ 1, 1, 1, 1, 1, 1 },
	mTravelLength(4),
	mTraveling(false)
{
	mTexture.loadFromFile(DEFAULT_TEXTURE);
	mSprite.setTexture(mTexture);
}

Entity::~Entity()
{

}

template <typename T>
void reverseVectorOrder(std::vector<T> &vector)
{
	auto tempVector = vector;
	vector.clear();
	while (!tempVector.empty())
	{
		vector.push_back(tempVector.back());
		tempVector.pop_back();
	}
}

void Entity::setTexture(const std::string & filename)
{
	mTexture.loadFromFile(filename);
}

void Entity::setEntityListener(IEntityListener * listener)
{
	mEntityListener = listener;
}

void Entity::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(mSprite, states);
}

float vectorMagnitude(const sf::Vector2f &vector)
{
	return (std::powf(vector.x * vector.x + vector.y * vector.y, .5f));
}

void normalizeVector(sf::Vector2f &vector)
{
	float magnitude = vectorMagnitude(vector);
	vector /= magnitude;
}

sf::Vector2f lerp(const sf::Vector2f &start, const sf::Vector2f &goal, float step, bool &reachedGoal)
{
	sf::Vector2f dir = goal - start;
	if (vectorMagnitude(dir) <= step)
	{
		reachedGoal = true;
		return goal;
	}
	else
	{
		normalizeVector(dir);
		reachedGoal = false;
		return (start + dir * step);
	}
}

void Entity::update(sf::Time & deltaTime)
{
	if (!mWaypoints.empty() && mTraveling)
	{
		bool reachedGoal;
		auto newPos = lerp(getPosition(), mWaypoints.back(), mInstantTravel ? 10000.f : deltaTime.asSeconds() * SPEED_FACTOR, reachedGoal);
		setPosition(newPos);
		if (reachedGoal)
		{
			mWaypoints.pop_back();
			if (mWaypoints.empty())
			{
				mTraveling = false;
				if (mEntityListener != nullptr)
					mEntityListener->destinationReached();
			}
		}
	}
}

sf::IntRect Entity::getRect()
{
	sf::Vector2i pos((int)getPosition().x, (int)getPosition().y);
	sf::Vector2i size(mSprite.getTextureRect().width, mSprite.getTextureRect().height);
	sf::IntRect rect(pos, size);
	return rect;
}

void Entity::setMovementPath(const std::vector<sf::Vector2f>& path, bool instantTravel)
{
	mWaypoints = path;
	reverseVectorOrder(mWaypoints);
	mTraveling = true;
	mInstantTravel = instantTravel;
}

void Entity::setMovementCosts(int * costs)
{
	for (int i = 0; i < Tile::TILE_TYPES_NR; i++)
	{
		mCosts[i] = costs[i];
	}
}

int Entity::getMovementCost(Tile::TileTypes tileType) const
{
	return mCosts[(int)tileType];
}

int* Entity::getMovementCosts()
{
	return mCosts;
}

void Entity::setTravelLength(int length)
{
	mTravelLength = length;
}

int Entity::getTravelLength() const
{
	return mTravelLength;
}
