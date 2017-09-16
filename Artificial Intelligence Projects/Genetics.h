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

	void initalializeFirstPopulation(size_t populationSize, float maxValueX, float maxValueY);
	void evolveNewGeneration();
	void applyFitness(float fitness, size_t index);
	Population getCurrentPopulation() const;

	void setTournamentSize(size_t size);
	void setRadialMutationSize(float radius);

	Genome getMostFitGenome() const;

private:
	Genome tournamentSelection(const Population &population);
	void combine(Genome &genome0, Genome &genome1);
	void mutate(Genome &genome);

	Population mCurrentPopulation;
	int mTournamentSize;
	float mRadialMutationSize;
	float mBoundMaxX, mBoundMinX, mBoundMinY, mBoundMaxY;
};