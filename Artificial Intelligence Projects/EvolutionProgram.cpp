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

static const char* DEFAULT_FONTFILE = "sui_generis_rg.ttf";

static const sf::Vector2f goalPos(64 * 18, 64 * 1);
static const sf::Vector2f startPos(64, 64);
static const sf::Vector2f halfTile(32, 32);

EvolutionProgram::EvolutionProgram() :
	mPopulationSize(60), mEpok(0)
{
	mFont.loadFromFile(DEFAULT_FONTFILE);
	mInfoText.setFont(mFont);
	mInfoText.setFillColor(sf::Color::Green);
	mInfoText.setPosition(64, 64);

	mPathRepresentation.setPrimitiveType(sf::LineStrip);
	mPathRepresentation.resize(6);
	for (int i = 0; i < 6; i++)
	{
		mPathRepresentation[i].color = sf::Color::Red;
	}
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
	sf::RenderWindow window(sf::VideoMode(64 * 20, 64 * 15), "Pathfinding AI");
	sf::Clock clock;
	int LEVEL[] = {
					1,1,4,4,1,2,2,3,3,3,3,3,3,3,3,3,2,0,1,0,
					0,4,1,4,4,2,2,3,3,3,3,3,3,3,2,2,1,4,4,4,
					0,4,1,4,4,2,2,3,3,3,3,3,3,3,2,0,4,4,4,0,
					4,4,4,4,2,2,3,3,3,3,3,3,3,2,2,4,4,4,1,3,
					4,4,4,2,2,2,3,3,3,3,3,3,2,2,2,5,4,4,0,3,
					4,4,4,5,2,2,2,3,3,3,3,2,2,2,2,5,5,2,2,3,
					0,4,5,5,5,2,2,2,3,3,3,2,2,1,4,5,5,2,3,3,
					2,2,2,5,5,4,1,2,2,2,2,2,1,1,4,4,4,2,3,3,
					3,3,2,2,4,4,4,1,2,2,2,4,0,1,4,4,0,2,3,3,
					3,3,3,2,4,4,4,4,4,5,5,4,4,4,4,4,0,2,3,3,
					3,3,2,0,4,4,1,4,4,5,5,5,4,4,4,1,4,2,3,3,
					3,3,2,0,4,1,1,4,2,2,2,2,2,0,4,0,2,3,3,3,
					3,3,3,2,0,0,4,2,2,2,3,3,2,2,2,2,3,3,3,3,
					3,3,3,3,2,2,2,2,3,3,3,3,3,3,3,3,3,3,3,3,
					3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3 };

	mTilemap.load(TILEMAP_TEXTUREFILE, sf::Vector2u(64, 64), LEVEL, 20, 15);

	// Setup wagon attributes
	int humanCosts[] = { 8, 13, 4, 25, 1, 1 };
	int humanTravelLength = 8;
	sf::Vector2i entityPos(1, 1);
	setupEntityAlso(mEntity, HUMAN_TEXTUREFILE, entityPos, humanCosts, humanTravelLength);
	mEntity.setTravelSpeed(5.0f);
	mEntity.setEntityListener(this);
	mInfoText.setString("Current epok: " + std::to_string(mEpok));

	mCurrentEvaluatingGenome = 0;
	mGenetics.initalializeFirstPopulation(60, 64 * 19, 64 * 14);
	mGenetics.setTournamentSize(4);
	mGenetics.setRadialMutationSize(64);

	mPathRepresentation[0].position = startPos + halfTile;
	mPathRepresentation[5].position = goalPos + halfTile;


	startGenomeEvaluation(mCurrentEvaluatingGenome);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::MouseButtonPressed)
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
			if (event.type == sf::Event::KeyPressed)
			{
				if (mWait && event.key.code == sf::Keyboard::Space)
				{
					mWait = false;
					mInfoText.setString("Current epok: " + std::to_string(mEpok));
					startGenomeEvaluation(mCurrentEvaluatingGenome);
				}
			}
		}
		// disregard of deltatime to have constant travellength per iteration
		sf::Time elapsed = sf::seconds(1.0f);
		if (!mWait)
		{
			update(elapsed);
			// Update fitness value of the entity
			float cost = (float)mEntity.getMovementCost((Tile::TileTypes)mTilemap.getTileTypeFromIndex(mTilemap.getIndexFromVector(mEntity.getPosition())));
			mFitnessEvaluation -=cost;
		}

		draw(window);
	}
}

void EvolutionProgram::update(sf::Time & deltaTime)
{
	mEntity.update(deltaTime);
}

void EvolutionProgram::draw(sf::RenderWindow & window)
{
	window.clear();
	window.draw(mTilemap);
	window.draw(mEntity);
	window.draw(mInfoText);
	if (mWait)
		window.draw(mPathRepresentation);
	window.display();
}

void EvolutionProgram::destinationReached()
{
	// Start evaluating the next genome in the population
	mGenetics.applyFitness(mFitnessEvaluation, mCurrentEvaluatingGenome);
	mCurrentEvaluatingGenome++;
	if (mCurrentEvaluatingGenome < mPopulationSize)
		startGenomeEvaluation(mCurrentEvaluatingGenome);
	else
	{
		// Current generation is fully evaluated, evolve a new generation;
		mCurrentEvaluatingGenome = 0;
		mWait = true;
		//mGenetics.se
		Genome genome = mGenetics.getMostFitGenome();
		mInfoText.setString("Epok " + std::to_string(mEpok) + " evaluated, Max fitness was " + std::to_string(genome.fitness) + "\nPress Space to continue");

		for (int i = 1; i < 5; i++)
		{
			mPathRepresentation[i].position = sf::Vector2f(genome.positions[(i - 1) * 2], genome.positions[(i - 1) * 2 + 1]) + halfTile;
		}
		mGenetics.evolveNewGeneration();
		mEpok++;
	}
}

void EvolutionProgram::startGenomeEvaluation(size_t index)
{
	mEntity.setPosition(startPos);
	Genetics::Population pop = mGenetics.getCurrentPopulation();

	std::vector<sf::Vector2f> movementPoints;
	int max = Genome::NR_POSITIONS / 2;
	for (size_t i = 0; i < max; i++)
		movementPoints.push_back(sf::Vector2f(pop[mCurrentEvaluatingGenome].positions[i * 2], pop[mCurrentEvaluatingGenome].positions[i * 2 + 1]));
	movementPoints.push_back(goalPos);

	mEntity.setMovementPath(movementPoints);
	mFitnessEvaluation = 2000.f;
}
