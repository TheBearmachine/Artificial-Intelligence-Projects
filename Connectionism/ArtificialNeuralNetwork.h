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

	void feedForward();
	// correctionArr has to be the same size as nr of output neurons
	void backPropagation(float learningSpeed, const float* correctionArr);

	Neuron* getInputNeuron(size_t index) const;
	Neuron* getOutputNeuron(size_t index) const;
	void addHiddenLayer(size_t size);

	void setWeight(float value, size_t layerIndex, size_t fromNueronIndex, size_t toNeuronIndex);
	float getWeight(size_t layerIndex, size_t fromNueronIndex, size_t toNeuronIndex) const;
	void randomizeWeights(float limit);

private:
	void clearLayers();
	void updateWeights();

	std::vector<Layer> mLayers;
	std::vector<std::vector<float>> mWeights;

};