#include "TilemapProgram.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Rect.hpp>

static const char* TILEMAP_TEXTUREFILE = "FancyTiles.png";
static const char* WAGON_TEXTUREFILE = "Wagon.png";
static const char* BOAT_TEXTUREFILE = "Boat.png";
static const char* HUMAN_TEXTUREFILE = "Human.png";
static const char* GOAT_TEXTUREFILE = "Goat.png";

TilemapProgram::TilemapProgram() :
	mEntityInTransit(false)
{
}

TilemapProgram::~TilemapProgram()
{
}

void setupEntity(Entity& entity, const char* textureFile, const sf::Vector2i &tilePos, int* movementCosts, int travelLength)
{
	entity.setTexture(textureFile);
	entity.setPosition((float)tilePos.x * 64.f, (float)tilePos.y * 64.f);
	entity.setMovementCosts(movementCosts);
	entity.setTravelLength(travelLength);
}

void TilemapProgram::run()
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
	int wagonCosts[] = { 3, 6, 5, 1000, 2, 1 };
	int travelLength = 8;
	setupEntity(mEntities[0], WAGON_TEXTUREFILE, sf::Vector2i(3, 5), wagonCosts, travelLength);
	// Setup boat attributes
	int boatCosts[] = { 1000, 1000, 1, 1, 1000, 1000 };
	travelLength = 6;
	setupEntity(mEntities[1], BOAT_TEXTUREFILE, sf::Vector2i(1, 8), boatCosts, travelLength);
	// Setup human attributes
	int humanCosts[] = { 2, 3, 2, 3, 1, 1 };
	travelLength = 6;
	setupEntity(mEntities[2], HUMAN_TEXTUREFILE, sf::Vector2i(6, 6), humanCosts, travelLength);
	// Setup goat attributes
	int goatCosts[] = { 2, 2, 3, 1000, 1, 2 };
	travelLength = 8;
	setupEntity(mEntities[3], GOAT_TEXTUREFILE, sf::Vector2i(2, 2), goatCosts, travelLength);


	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (!mEntityInTransit && event.type == sf::Event::MouseButtonPressed)
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
				else
				{
					auto path = mTilemap.getCurrentPath(sf::Vector2f(relativeMousePos));
					if (!path.empty())
					{
						mSelectedEntity->setMovementPath(path);
						mSelectedEntity->setEntityListener(this);
						mEntityInTransit = true;
					}
				}
			}
			if (!mEntityInTransit && event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Escape && mSelectedEntity != nullptr)
				{
					mTilemap.clearMoves();
					mSelectedEntity = nullptr;
				}
			}
			if (!mEntityInTransit && event.type == sf::Event::MouseMoved && mSelectedEntity != nullptr)
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

void TilemapProgram::update(sf::Time & deltaTime)
{
	mTilemap.update(deltaTime);
	for (int i = 0; i < 4; i++)
	{
		mEntities[i].update(deltaTime);
	}
}

void TilemapProgram::draw(sf::RenderWindow & window)
{
	window.clear();
	window.draw(mTilemap);
	for (int i = 0; i < 4; i++)
	{
		window.draw(mEntities[i]);
	}
	window.display();
}

void TilemapProgram::destinationReached()
{
	mEntityInTransit = false;
	mTilemap.clearMoves();
	mSelectedEntity = nullptr;
}
