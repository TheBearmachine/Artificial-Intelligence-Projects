#include "IdentityNeuron.h"

float IdentityNeuron::evaluate()
{
	return mInputValue;
}

float IdentityNeuron::evaluateDerivative()
{
	return 1.0f;
}
