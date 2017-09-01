#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexArray.hpp>

class Tilemap : public sf::Drawable, public sf::Transformable
{
public:
	Tilemap();
	~Tilemap();

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	bool load(const std::string& tileset, sf::Vector2u tileSize, const int* tiles, unsigned int width, unsigned int height);

private:
	sf::VertexArray mVertices;
	sf::Texture mTileset;
};