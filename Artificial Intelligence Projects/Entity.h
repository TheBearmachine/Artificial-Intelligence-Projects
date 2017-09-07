#pragma once
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Time.hpp>
#include <vector>
#include "Tile.h"

__interface IEntityListener
{
	void destinationReached();
};

class Entity : public sf::Transformable, public sf::Drawable
{
public:
	Entity();
	~Entity();

	void setTexture(const std::string &filename);
	void setEntityListener(IEntityListener* listener);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void update(sf::Time &deltaTime);
	sf::IntRect getRect();
	void setMovementPath(const std::vector<sf::Vector2f> &path);

	void setMovementCosts(int* costs);
	int getMovementCost(Tile::TileTypes tileType) const;
	int* getMovementCosts();

	void setTravelLength(int length);
	int getTravelLength() const;

private:
	sf::Sprite mSprite;
	sf::Texture mTexture;
	IEntityListener *mEntityListener;
	int mCosts[Tile::TILE_TYPES_NR];
	int mTravelLength;
	std::vector<sf::Vector2f> mWaypoints;
	bool mTraveling;
};