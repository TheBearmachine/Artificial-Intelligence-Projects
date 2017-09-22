#include "ArtificialNeuralNetwork.h"
#include "BiasNeuron.h"
#include "IdentityNeuron.h"
#include "SigmoidNeuron.h"
#include <random>

ArtificialNeuralNetwork::ArtificialNeuralNetwork(size_t inputSensors, size_t outputSensors) :
	mHasWeights(false)
{
	mLayers.push_back(Layer());
	mLayers[0].push_back(new BiasNeuron());
	for (size_t i = 0; i < inputSensors; i++)
	{
		mLayers[0].push_back(new IdentityNeuron());
	}

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

}

Neuron* ArtificialNeuralNetwork::getInputNeuron(size_t index)const
{
	if (mLayers[0].size() > index) return nullptr;
	return mLayers[0][index];
}

Neuron* ArtificialNeuralNetwork::getOutputNeuron(size_t index) const
{
	if (mLayers.back().size() > index) return nullptr;
	return mLayers.back()[index];
}

void ArtificialNeuralNetwork::addHiddenLayer(size_t size)
{
	mHasWeights = false;
	std::vector<Layer>::iterator it;
	it = mLayers.begin() + mLayers.size() - 2;
	mLayers.insert(it, Layer());
	mLayers.back().push_back(new BiasNeuron());
	for (size_t i = 0; i < size; i++)
	{
		mLayers[mLayers.size() - 2].push_back(new SigmoidNeuron());
	}
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
	for (auto first : mWeights)
	{
		for (auto second : first)
		{
			second = getRandomFloat(limit);
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

	mHasWeights = true;
}
