#pragma once
#include "StateEntity.h"
#include <SFML/Graphics/VertexArray.hpp>
#include "StatemachineIdle.h"
#include <queue>
#include <vector>

class StateEntityNonPlayable : public StateEntity
{
public:
	StateEntityNonPlayable();
	~StateEntityNonPlayable();

	void update(const sf::Time &deltaTime) override;
	virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

	void initalizePatrolQueue(const std::vector<sf::Vector2f> &points);

private:
	friend StatemachineIdle;
	friend Statemachine;

	Statemachine* mCurrentState;
	StatemachineIdle mStatemachineIdle;

	sf::VertexArray mSearchCone;
	float time;
	sf::Vector2f mLookVector;
	std::queue<sf::Vector2f> mPatrolPoints;
};