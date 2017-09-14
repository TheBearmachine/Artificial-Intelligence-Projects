#include "Genetics.h"
#include <random>

static const double COMBINATION_PROBABILITY = 0.3;
static const double MUTATION_PROBABILITY = 0.02;


Genetics::Genetics()
{
}

Genetics::~Genetics()
{
}


bool probability(double probability)
{
	double random = (double)(std::rand() % 10000) / 10000.0;
	return (random <= probability);
}

Genetics::Population Genetics::evolve(const Population & currentPopulation)
{
	Population newPop;
	while (newPop.size() < currentPopulation.size())
	{

		Genome genome0 = tournamentSelection(currentPopulation);
		Genome genome1 = tournamentSelection(currentPopulation);

		if (probability(COMBINATION_PROBABILITY))
		{
			combine(genome0, genome1);
		}
		mutate(genome0);
		mutate(genome1);

		if (newPop.size() <= currentPopulation.size())
			newPop.push_back(genome0);
		if (newPop.size() <= currentPopulation.size())
			newPop.push_back(genome1);
	}
	return newPop;
}

Genome Genetics::tournamentSelection(const Population & population)
{
	Genome newGen;
	std::vector<Genome> selectedIndividuals;
	size_t index;
	for (int i = 0; i < mTournamentSize; i++)
	{
		index = rand() % population.size();
		selectedIndividuals.push_back(population[index]);
	}

	index = population.size();
	float currentMaxFitness = -100.f;

	for (size_t i = 0; i < selectedIndividuals.size(); i++)
	{
		if (selectedIndividuals[i].fitness >= currentMaxFitness)
		{
			index = i;
			currentMaxFitness = selectedIndividuals[i].fitness;
		}
	}

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

void Genetics::mutate(Genome & genome)
{
	//genome.positions
	for (size_t i = 0; i < Genome::NR_POSITIONS; i++)
	{
		if (probability(MUTATION_PROBABILITY))
		{
			genome.positions[i] = derp;
		}
	}
}
