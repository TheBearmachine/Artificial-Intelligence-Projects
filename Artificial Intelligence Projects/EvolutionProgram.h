#pragma once
#include "Entity.h"
#include "Tilemap.h"
#include "Genetics.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

class EvolutionProgram : public IEntityListener
{
public:
	EvolutionProgram();
	~EvolutionProgram();

	void run();

private:
	void update(sf::Time &deltaTime);
	void draw(sf::RenderWindow& window);

	virtual void destinationReached() override;
	void startGenomeEvaluation(size_t index);

	Tilemap mTilemap;
	Entity mEntity;
	Genetics mGenetics;
	size_t mCurrentEvaluatingGenome;
	const size_t mPopulationSize;
	float mFitnessEvaluation;
	sf::Text mInfoText;
	sf::Font mFont;
	sf::VertexArray mPathRepresentation;
	bool mWait;

	size_t mEpoch;
};