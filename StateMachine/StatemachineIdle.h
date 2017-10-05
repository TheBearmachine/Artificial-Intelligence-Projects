#pragma once
#include "Statemachine.h"

class StatemachineIdle : public Statemachine
{
public:
	StatemachineIdle(StateEntityNonPlayable* owner);

	virtual void enter() override;
	virtual void exit() override;
	virtual void update(const sf::Time & deltaTime) override;
	virtual void startMovement(const sf::Vector2f &position) override;

private:

};