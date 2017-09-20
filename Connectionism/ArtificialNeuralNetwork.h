#pragma once
#include <vector>

class Neuron;

struct NeuronLink
{
public:
	void setNeurons(Neuron* n1, Neuron* n2) { this->n1 = n1; this->n2 = n2; }
	float weight;

private:
	Neuron *n1, *n2;
};

class ArtificialNeuralNetwork
{
public:
	typedef std::vector<Neuron*> Layer;

	ArtificialNeuralNetwork(size_t inputSensors, size_t outputSensors);
	~ArtificialNeuralNetwork();

	Neuron* getInputNeuron(size_t index) const;
	void addHiddenLayer(size_t size);

private:
	void clearLayer();
	void updateWeights();

	std::vector<Layer> mLayers;
	std::vector<std::vector<float>> mWeights;

	bool mHasWeights;
};