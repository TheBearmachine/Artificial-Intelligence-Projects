#include "StateEntity.h"
#include <SFML/Graphics/RenderTarget.hpp>

StateEntity::StateEntity()
{
}

StateEntity::~StateEntity()
{
}

void StateEntity::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(mSprite, states);
}
