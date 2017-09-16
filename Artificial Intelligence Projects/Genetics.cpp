#include "Genetics.h"
#include <random>

static const double COMBINATION_PROBABILITY = 0.3;
static const double MUTATION_PROBABILITY = 0.07;

Genetics::Genetics() :
	mCurrentPopulation(),
	mTournamentSize(), mRadialMutationSize()
{

}

Genetics::~Genetics()
{
}

// Returns true if a random value between 0 and 1 is less than probability
bool probability(double probability)
{
	double random = (double)(std::rand() % 10000) / 10000.0;
	return (random <= probability);
}

void Genetics::initalializeFirstPopulation(size_t populationSize, float maxValueX, float maxValueY)
{
	mCurrentPopulation.clear();
	mBoundMaxX = maxValueX;
	mBoundMaxY = maxValueY;
	mBoundMinX = 0.0f;
	mBoundMinY = 0.0f;
	while (mCurrentPopulation.size() < populationSize)
	{
		Genome newGenome;
		newGenome.fitness = 0.f;
		for (size_t i = 0; i < Genome::NR_POSITIONS; i++)
		{
			float randomVal1 = ((rand() % 10000) / 10000.f);
			float randomVal2 = ((rand() % 10000) / 10000.f);
			randomVal1 *= maxValueX;
			randomVal2 *= maxValueY;
			bool index1 = (i + 1) % 2;
			bool index2 = !index1;
			newGenome.positions[i] = randomVal1 * index1 + randomVal2 * index2;
		}
		mCurrentPopulation.push_back(newGenome);
	}
}

void Genetics::evolveNewGeneration()
{
	Population newPop;
	while (newPop.size() < mCurrentPopulation.size())
	{

		Genome genome0 = tournamentSelection(mCurrentPopulation);
		Genome genome1 = tournamentSelection(mCurrentPopulation);

		if (probability(COMBINATION_PROBABILITY))
		{
			combine(genome0, genome1);
		}
		mutate(genome0);
		mutate(genome1);

		genome0.fitness = 0;
		genome1.fitness = 0;

		if (newPop.size() <= mCurrentPopulation.size())
			newPop.push_back(genome0);
		if (newPop.size() <= mCurrentPopulation.size())
			newPop.push_back(genome1);
	}
	mCurrentPopulation = newPop;
}

void Genetics::applyFitness(float fitness, size_t index)
{
	mCurrentPopulation[index].fitness = fitness;
}

Genetics::Population Genetics::getCurrentPopulation() const
{
	return mCurrentPopulation;
}

void Genetics::setTournamentSize(size_t size)
{
	mTournamentSize = size;
}

void Genetics::setRadialMutationSize(float radius)
{
	mRadialMutationSize = radius;
}

Genome Genetics::getMostFitGenome() const
{
	Genome genome;
	size_t index = 0;
	float currentMaxFitness = -10000.f;
	// Find the most fit genome in the tournament
	for (size_t i = 0; i < mCurrentPopulation.size(); i++)
	{
		if (mCurrentPopulation[i].fitness >= currentMaxFitness)
		{
			index = i;
			currentMaxFitness = mCurrentPopulation[i].fitness;
		}
	}
	return mCurrentPopulation[index];
}

Genome Genetics::tournamentSelection(const Population & population)
{
	std::vector<Genome> selectedIndividuals;
	size_t index;
	for (int i = 0; i < mTournamentSize; i++)
	{
		index = rand() % population.size();
		selectedIndividuals.push_back(population[index]);
	}

	index = 0;
	float currentMaxFitness = -10000.f;

	// Find the most fit genome in the tournament
	for (size_t i = 0; i < selectedIndividuals.size(); i++)
	{
		if (selectedIndividuals[i].fitness >= currentMaxFitness)
		{
			index = i;
			currentMaxFitness = selectedIndividuals[i].fitness;
		}
	}

	Genome newGen = selectedIndividuals[index];
	return newGen;
}

// Combines genes from two genomes using two random indices and swapping all values
// between them
void Genetics::combine(Genome & genome0, Genome & genome1)
{
	size_t index0 = rand() % (Genome::NR_POSITIONS - 1);
	size_t index1 = rand() % (Genome::NR_POSITIONS - 1);
	while (index1 < index0)
	{
		index1 = (rand() % (Genome::NR_POSITIONS - 1)) + 1;
	}
	for (size_t i = index0; i < index1; i++)
	{
		float temp = genome0.positions[i];
		genome0.positions[i] = genome1.positions[i];
		genome1.positions[i] = temp;
	}
}

float mutateRadius(const float radius, const float &value, float minBound, float maxBound)
{
	float newVal = value;
	float randomFactor = ((rand() % 10000) / 5000.f) - 1.f; // Random value between -1.0 and 1.0
	randomFactor *= radius;
	newVal += radius;
	newVal = std::fmaxf(newVal, minBound);
	newVal = std::fminf(newVal, maxBound);

	return newVal;
}

// Will iterate over each gene in a genome and randomly mutate based on a radial scheme
void Genetics::mutate(Genome & genome)
{
	for (size_t i = 0; i < Genome::NR_POSITIONS; i++)
	{
		if (probability(MUTATION_PROBABILITY))
		{
			if (i % 2 == 0)
				genome.positions[i] = mutateRadius(mRadialMutationSize, genome.positions[i], mBoundMinX, mBoundMaxX);
			else
				genome.positions[i] = mutateRadius(mRadialMutationSize, genome.positions[i], mBoundMinY, mBoundMaxY);
		}
	}
}
