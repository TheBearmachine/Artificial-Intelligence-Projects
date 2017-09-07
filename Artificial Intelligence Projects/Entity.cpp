#include "Entity.h"
#include <SFML/Graphics/RenderWindow.hpp>

static const char* DEFAULT_TEXTURE = "DefaultImage.png";

Entity::Entity() :
	mCosts{ 1, 1, 1, 1, 1, 1 },
	mTravelLength(4)
{
	mTexture.loadFromFile(DEFAULT_TEXTURE);
	mSprite.setTexture(mTexture);
}

Entity::~Entity()
{
}

void Entity::setTexture(const std::string & filename)
{
	mTexture.loadFromFile(filename);
}

void Entity::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(mSprite, states);
}

void Entity::update(sf::Time & deltaTime)
{
}

sf::IntRect Entity::getRect()
{
	sf::Vector2i pos((int)getPosition().x, (int)getPosition().y);
	sf::Vector2i size(mSprite.getTextureRect().width, mSprite.getTextureRect().height);
	sf::IntRect rect(pos, size);
	return rect;
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
