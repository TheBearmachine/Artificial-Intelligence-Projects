#pragma once
#include "Statemachine.h"

class StatemachineMoving : public Statemachine
{
public:
	StatemachineMoving(StateEntityNonPlayable* owner);

	virtual void enter() override;
	virtual void exit() override;
	virtual void update(const sf::Time & deltaTime) override;

	virtual void startIdle(const float &idleTime) override;
	virtual void startChasing() override;


private:
	sf::Vector2f mTargetPosition;

};