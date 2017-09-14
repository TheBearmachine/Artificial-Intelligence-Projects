#include "TilemapGenerator.h"
#include <SFML/Window/Event.hpp>

static const char* TILEMAP_TEXTUREFILE = "FancyTiles.png";

TilemapGenerator::TilemapGenerator() :
	mPaintColor(0), mMouseButtonDown(false)
{
}

TilemapGenerator::~TilemapGenerator()
{
}

void TilemapGenerator::run(size_t nrTilesX, size_t nrTilesY)
{
	mTileIndexArray = new int[nrTilesX * nrTilesY];
	for (int i = 0; i < nrTilesX * nrTilesY; i++)
	{
		mTileIndexArray[i] = 0;
	}

	sf::RenderWindow window(sf::VideoMode(1920, 1080), "Pathfinding AI");
	sf::Clock clock;

	mTilemap.load(TILEMAP_TEXTUREFILE, sf::Vector2u(64, 64), mTileIndexArray, 15, 15);
	delete[nrTilesX * nrTilesY] mTileIndexArray;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			else if (event.type == sf::Event::MouseButtonPressed)
			{
				// Start painting
				sf::Vector2i mousePos(event.mouseButton.x, event.mouseButton.y);
				sf::Vector2f worldPos(window.mapPixelToCoords(mousePos));

				mTilemap.setTileTexture(worldPos, mPaintColor);
				mMouseButtonDown = true;
			}
			else if (event.type == sf::Event::MouseButtonReleased)
			{
				// Stop painting
				mMouseButtonDown = false;
			}
			else if (event.type == sf::Event::MouseMoved && mMouseButtonDown)
			{
				sf::Vector2i mousePos(event.mouseMove.x, event.mouseMove.y);
				sf::Vector2f worldPos(window.mapPixelToCoords(mousePos));

				mTilemap.setTileTexture(worldPos, mPaintColor);
			}
			else if (event.type == sf::Event::KeyPressed)
			{
				if ((int)event.key.code >= 26 && (int)event.key.code <= 31)
				{
					// Handle color swaps
					mPaintColor = (int)event.key.code - 26;
				}
				else if (event.key.code == sf::Keyboard::S)
				{
					// Save to file
				}
			}
		}
		update(clock.restart());
		draw(window);
	}
}

void TilemapGenerator::update(sf::Time & deltaTime)
{

}

void TilemapGenerator::draw(sf::RenderWindow & window)
{
	window.clear();
	window.draw(mTilemap);
	window.display();
}
