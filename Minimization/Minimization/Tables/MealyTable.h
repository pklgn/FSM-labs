#pragma once
#include <map>
#include <vector>
#include <string>
#include <utility>
#include <unordered_map>
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