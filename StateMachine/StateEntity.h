#pragma once
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Sprite.hpp>

class StateEntity : public sf::Transformable, public sf::Drawable
{
public:
	StateEntity();
	~StateEntity();

	virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const override;
	virtual void update(const sf::Time &deltaTime) = 0;

protected:
	sf::Sprite mSprite;
};
