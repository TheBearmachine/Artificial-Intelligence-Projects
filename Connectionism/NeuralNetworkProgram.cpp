#include "NeuralNetworkProgram.h"

#include <random>
#include <time.h>

static const size_t INPUT_SENSORS = 16;
static const size_t OUTPUT_SENSORS = 2;
static const size_t HIDDEN_LAYER_NR = 2;
static const size_t HIDDENLAYERS[HIDDEN_LAYER_NR] = { 8, 8 };
static const float WEIGHT_LIMIT = 10.f;

NeuralNetworkProgram::NeuralNetworkProgram() :
	mANN(INPUT_SENSORS, OUTPUT_SENSORS)
{
	
}

NeuralNetworkProgram::~NeuralNetworkProgram()
{
}

void NeuralNetworkProgram::run()
{
	std::srand(time(NULL));

	for (size_t i = 0; i < HIDDEN_LAYER_NR; i++)
	{
		mANN.addHiddenLayer(HIDDENLAYERS[i]);
	}mANN.randomizeWeights(WEIGHT_LIMIT);
}
