#pragma once
#include <map>
#include <vector>
#include <string>
#include <utility>
#include <unordered_map>
#include "FSMTable.hpp"

using MealyTransitionTable = std::unordered_map<State, StateTransitions>;

class MealyTable : public FSMTable<MealyTransitionTable>
{
public:
	MealyTable(States& states, InputSignals& inputSignals, MealyTransitionTable& transitionTable)
		: FSMTable(states, inputSignals, transitionTable)
	{
		for (auto& column : m_transitionTable)
		{
			column.second.renamedStates.resize(m_inputSignals.size());
		}
	}

	void Minimize() override;

protected:
	void RecursiveMinimize(size_t eqvClassesCount) override;

	void RemoveUnreachableStates() override;
};