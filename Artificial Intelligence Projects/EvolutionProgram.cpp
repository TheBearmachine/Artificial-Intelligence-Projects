#include "EvolutionProgram.h"
//#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <random>

static const char* TILEMAP_TEXTUREFILE = "FancyTiles.png";
static const char* WAGON_TEXTUREFILE = "Wagon.png";
static const char* BOAT_TEXTUREFILE = "Boat.png";
static const char* HUMAN_TEXTUREFILE = "Human.png";
static const char* GOAT_TEXTUREFILE = "Goat.png";

EvolutionProgram::EvolutionProgram() :
	mEntityInTransit(false)
{

}

EvolutionProgram::~EvolutionProgram()
{

}

void setupEntityAlso(Entity& entity, const char* textureFile, const sf::Vector2i &tilePos, int* movementCosts, int travelLength)
{
	entity.setTexture(textureFile);
	entity.setPosition((float)tilePos.x * 64.f, (float)tilePos.y * 64.f);
	entity.setMovementCosts(movementCosts);
	entity.setTravelLength(travelLength);
}

void EvolutionProgram::run()
{
	std::srand(time(NULL));

	sf::RenderWindow window(sf::VideoMode(640, 640), "Pathfinding AI");
	sf::Clock clock;
	int LEVEL[] = { 3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
					3,2,2,2,2,2,2,2,2,3,2,2,2,2,2,
					3,2,1,1,1,1,4,4,2,4,4,4,4,4,3,
					3,2,1,1,0,0,0,4,2,4,4,4,4,2,3,
					3,2,4,4,4,4,4,4,2,4,4,4,2,2,3,
					3,2,2,5,2,2,5,2,2,4,2,2,4,2,3,
					3,2,2,5,2,2,5,2,2,4,
					3,2,1,4,4,4,4,0,2,4,
					3,2,1,4,0,0,0,0,2,0,
					3,3,3,3,3,3,3,3,3,3 };

	mTilemap.load(TILEMAP_TEXTUREFILE, sf::Vector2u(64, 64), LEVEL, 15, 15);

	// Setup wagon attributes
	int humanCosts[] = { 3, 6, 5, 1000, 2, 1 };
	int humanTravelLength = 8;
	sf::Vector2i entityPos(5, 5);
	setupEntityAlso(mEntity, HUMAN_TEXTUREFILE, entityPos, humanCosts, humanTravelLength);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (!mEntityInTransit && event.type == sf::Event::MouseButtonPressed)
			{
				/*sf::Vector2i mousePos(event.mouseButton.x, event.mouseButton.y);
				sf::Vector2i relativeMousePos(window.mapPixelToCoords(mousePos));

				auto path = mTilemap.getCurrentPath(sf::Vector2f(relativeMousePos));
				if (!path.empty())
				{
					mEntity.setMovementPath(path, true);
					mEntity.setEntityListener(this);
					mEntityInTransit = true;
				}*/
			}
			if (!mEntityInTransit && event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Escape)
				{
					mTilemap.clearMoves();
				}
			}
		}

		sf::Time elapsed = clock.restart();
		update(elapsed);
		draw(window);
	}
}

void EvolutionProgram::update(sf::Time & deltaTime)
{
	//mTilemap.update(deltaTime);
	/*for (int i = 0; i < 4; i++)
	{
		mEntities[i].update(deltaTime);
	}*/
}

void EvolutionProgram::draw(sf::RenderWindow & window)
{
	window.clear();
	window.draw(mTilemap);
	window.draw(mEntity);
	window.display();
}

void EvolutionProgram::destinationReached()
{
	mEntityInTransit = false;
}
