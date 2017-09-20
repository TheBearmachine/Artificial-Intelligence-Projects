#pragma once
#include "Neuron.h"

class IdentityNeuron : public Neuron
{
public:
	virtual float evaluate() override;
	float evaluateDerivative() override;

};