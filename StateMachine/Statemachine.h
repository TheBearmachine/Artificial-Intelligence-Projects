#pragma once
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

class StateEntityNonPlayable;

class Statemachine
{
public:
	Statemachine(StateEntityNonPlayable* owner) : mOwner(owner) {}
	~Statemachine() {}

	virtual void transition(Statemachine* fromState, Statemachine* toState);
	virtual void enter() = 0;
	virtual void exit() = 0;
	virtual void update(const sf::Time &deltaTime) = 0;

	virtual void startMovement(const sf::Vector2f &position) {}
	virtual void startChasing() {}
	virtual void startIdle(const float &idleTime) {}

protected:
	StateEntityNonPlayable* mOwner;
};