#pragma once
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>

class StateEntity : public sf::Transformable, public sf::Drawable
{
public:
	StateEntity();
	~StateEntity();

private:

};