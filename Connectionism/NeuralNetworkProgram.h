#pragma once
#include "ArtificialNeuralNetwork.h"

class NeuralNetworkProgram
{
public:
	NeuralNetworkProgram();
	~NeuralNetworkProgram();

	void run();

private:
	ArtificialNeuralNetwork mANN;

	bool mRunning;
};
