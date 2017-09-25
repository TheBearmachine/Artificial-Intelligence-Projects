#pragma once

class Neuron
{
public:
	Neuron() : mInputValue(0.0f) {}
	virtual float evaluate() = 0;
	virtual float evaluateDerivative() = 0;

	virtual void setInputValue(float val) { mInputValue = val; }
	virtual void addInputValue(float val) { mInputValue += val; }
	virtual float getInputValue() const { return mInputValue; }

	void setDeltaValue(float value) { mDeltaValue = value; }
	float getDeltaValue() const { return mDeltaValue; }

protected:
	float mInputValue;
	float mDeltaValue;
};