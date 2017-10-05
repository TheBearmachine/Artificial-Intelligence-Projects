#include "Statemachine.h"
#include "StateEntityNonPlayable.h"

void Statemachine::transition(Statemachine * fromState, Statemachine * toState)
{
	 fromState->exit(); toState->enter();
	 mOwner->mCurrentState = toState;
}
