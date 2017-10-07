#pragma once
#include "StateEntity.h"
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include "StatemachineIdle.h"
#include "StatemachineMoving.h"
#include "StatemachineChasing.h"
#include <queue>
#include <vector>

class StateEntityPlayable;

class StateEntityNonPlayable : public StateEntity
{
public:
	StateEntityNonPlayable();
	~StateEntityNonPlayable();

	void setPlayerReference(StateEntityPlayable *player);

	void update(const sf::Time &deltaTime) override;
	virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

	bool moveInDirection(const sf::Vector2f &direction, const float &deltaTime);
	void updateSightIndicator(const sf::Vector2f &direction);
	void setIndicator(const std::string &str);

	void initalizePatrolQueue(const std::vector<sf::Vector2f> &points);

private:
	friend StatemachineIdle;
	friend StatemachineChasing;
	friend StatemachineMoving;
	friend Statemachine;

	Statemachine* mCurrentState;
	StatemachineIdle mStatemachineIdle;
	StatemachineMoving mStatemachineMoving;
	StatemachineChasing mStatemachineChasing;

	sf::VertexArray mSearchCone;
	sf::Text mIndicator;
	sf::Font mIndicatorFont;
	float time;
	sf::Vector2f mLookVector;
	//sf::Vector2f mForwardVector;
	std::queue<sf::Vector2f> mPatrolPoints;

	sf::Vector2f mLastKnownPlayerPosition;
	StateEntityPlayable *mPlayer;

	const float CHASE_TIME_MAX = 2.0f;
	const float IDLE_TIME_MAX = 3.0f;
};