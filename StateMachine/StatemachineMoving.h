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

	void setTargetPosition(const sf::Vector2f &tarPos);

private:
	sf::Vector2f mTargetPosition;
};