#include "MealyTable.h"

States MealyTable::GetStates() const
{
	return m_states;
}

InputSignals MealyTable::GetInputSignals() const
{
	return m_inputSignals;
}

TransitionTable MealyTable::GetTransitionTable() const
{
	return m_transitionTable;
}

void MealyTable::SetStates(const States& states)
{
	m_states = states;
}

void MealyTable::SetInputSignals(const InputSignals& inputSignals)
{
	m_inputSignals = inputSignals;
}

void MealyTable::SetTransitionTable(const TransitionTable& transitionTable)
{
	m_transitionTable = transitionTable;
}
