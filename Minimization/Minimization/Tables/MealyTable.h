#pragma once
#include <map>
#include <vector>
#include <string>
#include <utility>
#include <unordered_map>
#include "FSMTable.hpp"

using StateTransitions = std::vector<StateTransition>;
using MealyTransitionTable = std::unordered_map<State, StateTransitions>;

class MealyTable : public FSMTable<MealyTransitionTable>
{
public:
	MealyTable(States& states, InputSignals& inputSignals, MealyTransitionTable& transitionTable)
		: FSMTable(states, inputSignals, transitionTable)
	{
	}

	void Minimize() override;

protected:
	void RecursiveMinimize() override;

	void RemoveUnreachableStates() override;
};