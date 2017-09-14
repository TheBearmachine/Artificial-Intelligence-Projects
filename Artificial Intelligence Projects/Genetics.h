#pragma once
#include <vector>
#include <SFML/System/Vector2.hpp>

struct Genome
{
public:
	static const size_t NR_POSITIONS = 8;
	float positions[NR_POSITIONS];
	float fitness;
};

class Genetics
{
public:
	typedef std::vector<Genome> Population;

	Genetics();
	~Genetics();

	Population evolve(const Population &currentPopulation);

private:
	Genome tournamentSelection(const Population &population);
	void combine(Genome &genome0, Genome &genome1);
	void mutate(Genome &genome);

	Population mCurrentPopulation;
	int mTournamentSize;
	float radialMutationSize;
};