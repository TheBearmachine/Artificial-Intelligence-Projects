#pragma once
#include "Neuron.h"

class SigmoidNeuron : public Neuron
{
public:
	virtual float evaluate() override;
	float evaluateDerivative() override;

};