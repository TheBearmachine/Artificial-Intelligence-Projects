#include "NeuralNetworkProgram.h"

#include <random>
#include <time.h>
#include <iostream>
#include <iomanip>
#include "BiasNeuron.h"
#include "IdentityNeuron.h"
#include "SigmoidNeuron.h"

static const size_t INPUT_SENSOR_NR = 16;

static const float INPUT_SHAPE_CIRCLE[INPUT_SENSOR_NR]{
	0,1,1,0,
	1,0,0,1,
	1,0,0,1,
	0,1,1,0
};

static const float INPUT_SHAPE_CIRCLE_BROKEN[INPUT_SENSOR_NR]{
	1,0,1,0,
	1,0,0,1,
	1,0,0,1,
	0,1,1,0
};

static const float INPUT_SHAPE_CROSS[INPUT_SENSOR_NR]{
	1,0,0,1,
	0,1,1,0,
	0,1,1,0,
	1,0,0,1
};

static const float INPUT_SHAPE_CROSS_BROKEN[INPUT_SENSOR_NR]{
	1,0,1,1,
	0,0,1,0,
	0,1,1,0,
	1,0,0,1
};

static const float INPUT_SHAPE_STUPID_LINE[INPUT_SENSOR_NR]{
	0,0,1,1,
	0,0,1,1,
	0,1,1,0,
	1,1,0,0
};

static const size_t OUTPUT_SENSOR_NR = 2;
static const float EXPECTED_OUTPUT_CIRCLE[OUTPUT_SENSOR_NR]
{ 0.0, 1.0 };

static const float EXPECTED_OUTPUT_CROSS[OUTPUT_SENSOR_NR]
{ 1.0, 0.0 };

static const size_t HIDDEN_LAYER_NR = 1;
static const size_t HIDDENLAYERS[HIDDEN_LAYER_NR] = {4};

static const float WEIGHT_LIMIT = 1.f;

static const float LEARNING_SPEED = 0.005;

NeuralNetworkProgram::NeuralNetworkProgram() :
	mANN(INPUT_SENSOR_NR, OUTPUT_SENSOR_NR),
	mRunning(true)
{

}

NeuralNetworkProgram::~NeuralNetworkProgram()
{
}

void NeuralNetworkProgram::run()
{
	std::srand(time(NULL));

	Neuron* inputNeurons[INPUT_SENSOR_NR];
	Neuron* outputNeurons[OUTPUT_SENSOR_NR];

	std::cout << std::setprecision(2) << std::fixed;

	/*for (size_t i = 0; i < HIDDEN_LAYER_NR; i++)
	{
		mANN.addHiddenLayer(HIDDENLAYERS[i]);
	}*/

	for (size_t i = 0; i < INPUT_SENSOR_NR; i++)
	{
		mANN.getInputNeuron(i)->setInputValue(INPUT_SHAPE_CIRCLE[i]);
		inputNeurons[i] = mANN.getInputNeuron(i);
	}

	for (size_t i = 0; i < OUTPUT_SENSOR_NR; i++)
	{
		outputNeurons[i] = mANN.getOutputNeuron(i);
	}
	mANN.randomizeWeights(WEIGHT_LIMIT);
	mANN.getOutputNeuron(0);

	// Train the network to recognize a circle
	for (size_t i = 0; i < INPUT_SENSOR_NR; i++)
	{
		mANN.getInputNeuron(i)->setInputValue(INPUT_SHAPE_CIRCLE[i]);
	}
	for (size_t i = 0; i < 6000; i++)
	{
		mANN.feedForward(); mANN.backPropagation(LEARNING_SPEED, EXPECTED_OUTPUT_CIRCLE);
	}

	// Train the network to recognize a cross
	for (size_t i = 0; i < INPUT_SENSOR_NR; i++)
	{
		mANN.getInputNeuron(i)->setInputValue(INPUT_SHAPE_CROSS[i]);
	}
	for (size_t i = 0; i < 6000; i++)
	{
		mANN.feedForward(); mANN.backPropagation(LEARNING_SPEED, EXPECTED_OUTPUT_CROSS);
	}

	size_t input = 0;
	while (mRunning)
	{
		std::cout << "\nInput: 0 for a circle shape, 1 for a cross shape\n2 for a broken circle, 3 for a broken cross, 4 for a line thing\n";
		std::cin >> input;
		switch (input)
		{
		case 0:
			for (size_t i = 0; i < INPUT_SENSOR_NR; i++)
			{
				mANN.getInputNeuron(i)->setInputValue(INPUT_SHAPE_CIRCLE[i]);
			}
			break;

		case 1:
			for (size_t i = 0; i < INPUT_SENSOR_NR; i++)
			{
				mANN.getInputNeuron(i)->setInputValue(INPUT_SHAPE_CROSS[i]);
			}
			break;

		case 2:
			for (size_t i = 0; i < INPUT_SENSOR_NR; i++)
			{
				mANN.getInputNeuron(i)->setInputValue(INPUT_SHAPE_CIRCLE_BROKEN[i]);
			}
			break;

		case 3:
			for (size_t i = 0; i < INPUT_SENSOR_NR; i++)
			{
				mANN.getInputNeuron(i)->setInputValue(INPUT_SHAPE_CROSS_BROKEN[i]);
			}
			break;

		case 4:
			for (size_t i = 0; i < INPUT_SENSOR_NR; i++)
			{
				mANN.getInputNeuron(i)->setInputValue(INPUT_SHAPE_STUPID_LINE[i]);
			}
			break;

		default:
			mRunning = false;
			return;
			break;
		}
		mANN.feedForward();

		static size_t breakpoint = pow(INPUT_SENSOR_NR, .5f);
		std::cout << "Input sensors:\n";
		for (size_t i = 0; i < INPUT_SENSOR_NR; i++)
		{
			if (i % breakpoint == 0) std::cout << std::endl;
			std::cout << inputNeurons[i]->evaluate() << " ";
		}
		std::cout << "\n\nOutput sensors:\n";
		for (size_t i = 0; i < OUTPUT_SENSOR_NR; i++)
		{
			std::cout << outputNeurons[i]->evaluate() << " ";
		}

		std::cout << std::endl;
		if (outputNeurons[1]->evaluate() > 0.7f && outputNeurons[0]->evaluate() < 0.3f)
		{
			std::cout << "The shape is considered a circle\n\n";
		}
		else if (outputNeurons[0]->evaluate() > 0.7f && outputNeurons[1]->evaluate() < 0.3f)
		{
			std::cout << "The shape is considered a cross\n\n";
		}
		else
		{
			std::cout << "The shape doesn't match any training examples\n\n";
		}

	}
}
