#include "ArtificialNeuralNetwork.h"
#include "BiasNeuron.h"
#include "IdentityNeuron.h"
#include "SigmoidNeuron.h"
#include <random>

ArtificialNeuralNetwork::ArtificialNeuralNetwork(size_t inputSensors, size_t outputSensors)
{
	mLayers.push_back(Layer());
	for (size_t i = 0; i < inputSensors; i++)
	{
		mLayers[0].push_back(new IdentityNeuron());
	}
	mLayers[0].push_back(new BiasNeuron());

	mLayers.push_back(Layer());
	for (size_t i = 0; i < outputSensors; i++)
	{
		mLayers[1].push_back(new SigmoidNeuron());
	}
}

ArtificialNeuralNetwork::~ArtificialNeuralNetwork()
{
	clearLayers();
}

void ArtificialNeuralNetwork::feedForward()
{
	for (size_t i = 1; i < mLayers.size(); i++)
	{
		for (size_t j = 0; j < mLayers[i].size(); j++)
		{
			mLayers[i][j]->setInputValue(0.f);
			for (size_t k = 0; k < mLayers[i - 1].size(); k++)
			{
				float weight = getWeight(i - 1, k, j);
				float value = mLayers[i - 1][k]->evaluate();
				mLayers[i][j]->addInputValue(weight * value);
			}
			mLayers[i][j]->evaluate();
		}
	}
}

void ArtificialNeuralNetwork::backPropagation(float learningSpeed, const float* correctionArr)
{
	// Update output neurons first
	size_t backIndex = mLayers.size() - 1;
	for (size_t i = 0; i < mLayers[backIndex].size(); i++)
	{
		Neuron* neu = mLayers[backIndex][i];
		float error = correctionArr[i] - neu->evaluate();
		float deltaVal = neu->evaluateDerivative() * error;
		neu->setDeltaValue(deltaVal);

		for (size_t j = 0; j < mLayers[backIndex - 1].size(); j++)
		{
			float weight = getWeight(backIndex - 1, j, i);
			float output = mLayers[backIndex - 1][j]->evaluate();
			setWeight(weight + learningSpeed * output * deltaVal, backIndex - 1, j, i);
		}
	}

	// Iterate backwards through all hidden layers
	for (size_t i = mLayers.size() - 2; i > 1; i--)
	{
		for (size_t j = 0; j < mLayers[i].size(); j++)
		{
			Neuron* neu = mLayers[i][j];
			float deltaVal = neu->evaluateDerivative();
			float sumWeightDelta = 0.f;

			for (size_t k = 0; k < mLayers[i + 1].size(); k++)
			{
				float delta = mLayers[i + 1][k]->getDeltaValue();
				float weight = getWeight(i, j, k);
				sumWeightDelta += delta * weight;
			}
			deltaVal *= sumWeightDelta;
			neu->setDeltaValue(deltaVal);

			for (size_t k = 0; k < mLayers[i - 1].size(); k++)
			{
				float oldWeight = getWeight(i - 1, k, j);
				float output = mLayers[i - 1][k]->evaluate();
				float newWeight = oldWeight + learningSpeed * output * deltaVal;
			}
		}
	}
}

Neuron* ArtificialNeuralNetwork::getInputNeuron(size_t index)const
{
	if (mLayers[0].size() < index) return nullptr;
	return mLayers[0][index];
}

Neuron* ArtificialNeuralNetwork::getOutputNeuron(size_t index) const
{
	if (mLayers.back().size() < index) return nullptr;
	return mLayers.back()[index];
}

void ArtificialNeuralNetwork::addHiddenLayer(size_t size)
{
	std::vector<Layer>::iterator it;
	it = mLayers.begin() + mLayers.size() - 1;
	mLayers.insert(it, Layer());
	for (size_t i = 0; i < size; i++)
	{
		mLayers[mLayers.size() - 2].push_back(new SigmoidNeuron());
	}
	mLayers[mLayers.size() - 2].push_back(new BiasNeuron());
	updateWeights();
}

void ArtificialNeuralNetwork::setWeight(float value, size_t layerIndex, size_t fromNueronIndex, size_t toNeuronIndex)
{
	auto sizex = mLayers[layerIndex].size();
	auto sizey = mLayers[layerIndex + 1].size();
	auto index = toNeuronIndex + fromNueronIndex * sizey;

	mWeights[layerIndex][index] = value;
}

float ArtificialNeuralNetwork::getWeight(size_t layerIndex, size_t fromNueronIndex, size_t toNeuronIndex) const
{
	auto sizex = mLayers[layerIndex].size();
	auto sizey = mLayers[layerIndex + 1].size();
	auto index = toNeuronIndex + fromNueronIndex * sizey;

	return mWeights[layerIndex][index];
}

float getRandomFloat(float limit)
{
	float value = ((rand() % 10000) / 5000.f) - 1.0f;
	return value * limit;
}

void ArtificialNeuralNetwork::randomizeWeights(float limit)
{
	updateWeights();
	for (size_t i = 0; i < mWeights.size(); i++)
	{
		for (size_t j = 0; j < mWeights[i].size(); j++)
		{
			float val = getRandomFloat(limit);
			mWeights[i][j] = val;
		}
	}
}

void ArtificialNeuralNetwork::clearLayers()
{
	while (!mLayers.empty())
	{
		while (!mLayers.back().empty())
		{
			delete mLayers.back().back();
			mLayers.back().pop_back();
		}
		mLayers.pop_back();
	}
}

void ArtificialNeuralNetwork::updateWeights()
{
	mWeights.clear();
	for (size_t i = 0; i < mLayers.size() - 1; i++)
	{
		mWeights.push_back(std::vector<float>());
		for (size_t j = 0; j < mLayers[i].size(); j++)
		{
			for (size_t k = 0; k < mLayers[i + 1].size(); k++)
			{
				mWeights[i].push_back(0.0f);
			}
		}
	}
}
