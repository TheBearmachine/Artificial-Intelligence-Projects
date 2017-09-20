#include "ArtificialNeuralNetwork.h"
#include "BiasNeuron.h"
#include "IdentityNeuron.h"
#include "SigmoidNeuron.h"

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
	while (!mLayers.empty())
	{
		clearLayer();
	}
}

Neuron* ArtificialNeuralNetwork::getInputNeuron(size_t index)const
{
	if (mLayers[0].size() > index) return nullptr;
	return mLayers[0][index];
}

void ArtificialNeuralNetwork::addHiddenLayer(size_t size)
{
	mHasWeights = false;
	mLayers.push_back(Layer());
	mLayers.back.push_back(new BiasNeuron());
	for (size_t i = 0; i < size; i++)
	{
		mLayers.back().push_back(new SigmoidNeuron());
	}
}

void ArtificialNeuralNetwork::clearLayer()
{
	while (!mLayers.back().empty())
	{
		delete mLayers.back().back();
		mLayers.back.pop_back();
	}
	mLayers.pop_back();
}

void ArtificialNeuralNetwork::updateWeights()
{
	mWeights.clear();
	for (size_t i = 0; i < mLayers.size() - 1; i++)
	{
		for (size_t j = 0; j < mLayers[i].size(); j++)
		{
			for (size_t k = 0; k < mLayers[i + 1].size(); k++)
			{

			}
		}
	}


	mHasWeights = true;
}
