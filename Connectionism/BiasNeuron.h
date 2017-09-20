#pragma once
#include "Neuron.h"

class BiasNeuron : public Neuron
{
public:
	virtual float evaluate() override;
	float evaluateDerivative() override;

};