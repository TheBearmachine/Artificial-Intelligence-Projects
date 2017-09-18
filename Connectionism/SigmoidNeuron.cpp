#include "SigmoidNeuron.h"
#include <cmath>

float SigmoidNeuron::evaluate()
{
	return 1.0f/( 1.0f + std::exp(mInputValue));
}
