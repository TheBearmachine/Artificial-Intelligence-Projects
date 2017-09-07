#pragma once
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Time.hpp>
#include "Tile.h"


class Entity : public sf::Transformable, public sf::Drawable
{
public:
	Entity();
	~Entity();

	void setTexture(const std::string &filename);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void update(sf::Time &deltaTime);
	sf::IntRect getRect();

	void setMovementCosts(int* costs);
	int getMovementCost(Tile::TileTypes tileType) const;
	int* getMovementCosts();

	void setTravelLength(int length);
	int getTravelLength() const;

private:
	sf::Sprite mSprite;
	sf::Texture mTexture;
	int mCosts[Tile::TILE_TYPES_NR];
	int mTravelLength;
};