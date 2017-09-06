#include "TilemapProgram.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Rect.hpp>

static const char* TILEMAP_TEXTUREFILE = "FancyTiles.png";
static const char* WAGON_TEXTUREFILE = "Wagon.png";

TimemapProgram::TimemapProgram()
{
}

TimemapProgram::~TimemapProgram()
{
}

void TimemapProgram::run()
{
	sf::RenderWindow window(sf::VideoMode(640, 640), "Pathfinding AI");
	sf::Clock clock;
	int LEVEL[] = { 3,3,3,3,3,3,3,3,3,3,
					3,2,2,2,2,2,2,2,2,3,
					3,2,1,1,1,1,4,4,2,3,
					3,2,1,1,0,0,0,4,2,3,
					3,2,4,4,4,4,4,4,2,3,
					3,2,2,5,2,2,5,2,2,3,
					3,2,2,5,2,2,5,2,2,3,
					3,2,1,4,4,4,4,0,2,3,
					3,2,1,4,0,0,0,0,2,3,
					3,3,3,3,3,3,3,3,3,3 };

	mTilemap.load(TILEMAP_TEXTUREFILE, sf::Vector2u(64, 64), LEVEL, 10, 10);

	// Setup wagon attributes
	mEntities[0].setPosition(64 * 3, 64 * 5);
	mEntities[0].setTexture(WAGON_TEXTUREFILE);
	int wagonCosts[] = { 3, 6, 5, 1000, 1, 1 };
	int wagonTravelLength = 8;
	mEntities[0].setMovementCosts(wagonCosts);
	mEntities[0].setTravelLength(wagonTravelLength);

	// Setup x attributes

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::MouseButtonPressed)
			{
				sf::Vector2i mousePos(event.mouseButton.x, event.mouseButton.y);
				sf::Vector2i relativeMousePos(window.mapPixelToCoords(mousePos));
				if (mSelectedEntity == nullptr)
				{
					for (int i = 0; i < 4; i++)
					{
						if (mEntities[i].getRect().contains(relativeMousePos))
						{
							mSelectedEntity = &mEntities[i];
							// Calculate available paths by adding all nodes with a travelcost
							mTilemap.calculateAvailableMoves(mSelectedEntity->getPosition(), mSelectedEntity->getTravelLength(), mSelectedEntity->getMovementCosts());
							printf("Selected entity %i\n", i);
							break;
						}
					}
				}
			}
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Escape && mSelectedEntity != nullptr)
				{
					mTilemap.clearPaths();
					mSelectedEntity = nullptr;
				}
			}
			if (event.type == sf::Event::MouseMoved && mSelectedEntity != nullptr)
			{
				sf::Vector2i mousePos(event.mouseMove.x, event.mouseMove.y);
				sf::Vector2i relativeMousePos(window.mapPixelToCoords(mousePos));

				mTilemap.calculatePath(sf::Vector2f((float)relativeMousePos.x, (float)relativeMousePos.y));
			}
		}

		sf::Time elapsed = clock.restart();
		update(elapsed);
		draw(window);

	}
}

void TimemapProgram::update(sf::Time & deltaTime)
{
	mTilemap.update(deltaTime);
}

void TimemapProgram::draw(sf::RenderWindow & window)
{
	window.clear();
	window.draw(mTilemap);
	for (int i = 0; i < 4; i++)
	{
		window.draw(mEntities[i]);
	}
	window.display();
}
