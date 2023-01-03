#include <deque>
#include <set>
#include <iostream>
#include "MooreTable.h"

const State NO_STATE = "";
const State INITIAL_STATE = "H";
const State NO_STATE_VISIBLE = "-";

Signals MooreTable::GetOutputSignals() const
{
	return m_outputSignals;
}

MooreStateTransitions MooreTable::GetMooreStates() const
{
	return m_mooreStates;
}

void MooreTable::Minimize()
{
	std::unordered_map<Signal, EquivalenceClass> equivalenceClasses;

	size_t equivalenceClass = 0;
	for (auto& outputSignal : m_outputSignals)
	{
		auto insertResult = equivalenceClasses.emplace(outputSignal, equivalenceClass);

		if (insertResult.second)
		{
			++equivalenceClass;
		}
	}

	for (auto& [state, outputSignal] : m_mooreStates)
	{
		m_eqvClasses[state] = equivalenceClasses[outputSignal];
	}

	CompleteCommonMinimization(equivalenceClasses.size());

	m_outputSignals.clear();
	for (auto& [oldState, outputSignal] : m_mooreStates)
	{
		if (m_eqvClasses.count(oldState))
		{
			m_outputSignals.push_back(outputSignal);
		}
	}
}

void MooreTable::RenameStates(const std::string& prefix)
{
	std::unordered_map<State, State> renamedStates;
	size_t index = 0;
	for (auto&& state : m_states)
	{
		if (state == " ")
		{
			renamedStates.emplace(state, INITIAL_STATE);
			std::cout << "Added initial state "
					  << renamedStates[state] << '\n';
		}
		else
		{
			renamedStates.emplace(state, prefix + std::to_string(index));
			std::cout << renamedStates[state]
					  << " = "
					  << state << '\n';
			++index;
		}
		state = renamedStates[state];
	}

	MooreTransitionTable renamedTransitionTable;
	for (auto&& [srcState, transitions] : m_transitionTable)
	{
		for (auto&& state : transitions.commonStates)
		{
			if (state == NO_STATE)
			{
				state = NO_STATE_VISIBLE;
			}
			else
			{
				state = renamedStates[state];
			}
		}
		renamedTransitionTable[renamedStates[srcState]] = transitions;
	}
	m_transitionTable = renamedTransitionTable;
}

void MooreTable::RemoveUnreachableOutputSignals(const States& oldStates)
{
	/*
	* passing through the elements from the end because we will always 
	* have a not deleted first state
	*/
	auto oldStateIndex = oldStates.size() - 1;
	auto currStateIndex = m_states.size() - 1;
	while (oldStateIndex > 0)
	{
		if (oldStates[oldStateIndex] != m_states[currStateIndex])
		{
			m_outputSignals.erase(m_outputSignals.begin() + oldStateIndex);
			--oldStateIndex;
		}
		else
		{
			--oldStateIndex;
			--currStateIndex;
		}
	}
}
