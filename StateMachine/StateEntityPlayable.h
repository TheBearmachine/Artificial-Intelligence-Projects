#pragma once
#include "StateEntity.h"
#include<string>

class StateEntityPlayable : public StateEntity
{
public:
	StateEntityPlayable();
	~StateEntityPlayable();


	void update(const sf::Time &deltaTime) override;

private:

};
