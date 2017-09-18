#pragma once

class Neuron
{
public:
	Neuron() : mInputValue(0.0f) {}
	virtual float evaluate() = 0;
	virtual void setInputValue(float val) { mInputValue = val; }
	virtual float getInputValue() const { return mInputValue; }

protected:
	float mInputValue;
};