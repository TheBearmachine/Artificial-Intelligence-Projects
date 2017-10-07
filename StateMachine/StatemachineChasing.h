#pragma once
#include "Statemachine.h"

class StatemachineChasing : public Statemachine
{
public:
	StatemachineChasing(StateEntityNonPlayable* owner);

	virtual void enter() override;
	virtual void exit() override;
	virtual void update(const sf::Time & deltaTime) override;

	virtual void startMovement(const sf::Vector2f &position) override;
	virtual void startChasing() override;

private:
	float mChaseTimer;
};