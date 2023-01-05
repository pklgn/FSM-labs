#pragma once
#include "FSMTable.hpp"

class MealyTable : public FSMTable<MealyTransitionTable>
{
public:
	MealyTable(States& states, Signals& inputSignals, MealyTransitionTable& transitionTable)
		: FSMTable(states, inputSignals, transitionTable)
	{
	}

	void Minimize() override;
};