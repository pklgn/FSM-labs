#include <deque>
#include <set>
#include <algorithm>
#include "MealyTable.h"

void MealyTable::Minimize()
{
	std::vector<std::vector<Signal>> outputSignals;
	outputSignals.resize(m_transitionTable.size());
	size_t columnIndex = 0;
	for (auto& column : m_transitionTable)
	{
		for (auto& field : column.second)
		{
			outputSignals[columnIndex].push_back(field.outputSignal);
		}

		++columnIndex;
	}

	std::vector<std::vector<Signal>> uniqueOutputSignals = outputSignals;
	std::sort(uniqueOutputSignals.begin(), uniqueOutputSignals.end());
	uniqueOutputSignals.erase(std::unique(uniqueOutputSignals.begin(), uniqueOutputSignals.end()), uniqueOutputSignals.end());

	auto uniqueOutputSignalsIt = uniqueOutputSignals.begin();
	for (auto& uniqueOutputSignal : uniqueOutputSignals)
	{
		auto it = outputSignals.begin();
		while ((it = std::find(it, outputSignals.end(), uniqueOutputSignal)) != outputSignals.end())
		{
			auto columnIndex = std::distance(outputSignals.begin(), it);
			m_eqvClasses[m_states[columnIndex]] = std::distance(uniqueOutputSignals.begin(), uniqueOutputSignalsIt);

			it++;
		}

		++uniqueOutputSignalsIt;
	}

	RecursiveMinimize();
}

void MealyTable::RecursiveMinimize()
{
	for (auto& column : m_transitionTable)
	{
		for (auto& transition: column.second)
		{
			transition.state = std::to_string(m_eqvClasses[transition.state]);
		}
	}

}

void MealyTable::RemoveUnreachableStates()
{
	State inspectedState = m_states.front();

	std::set<State> traversedStates;
	std::set<State> reachableStates;
	std::deque<State> statesWave;

	reachableStates.insert(inspectedState);
	for (auto& mealyState : m_transitionTable[inspectedState])
	{
		if (inspectedState != mealyState.state)
		{
			statesWave.push_back(mealyState.state);
			reachableStates.insert(mealyState.state);
		}
	}
	traversedStates.insert(inspectedState);

	while (!statesWave.empty())
	{
		inspectedState = statesWave.front();
		traversedStates.insert(inspectedState);
		for (auto& transitionState : m_transitionTable[inspectedState])
		{
			State destinationState = transitionState.state;
			if (inspectedState != destinationState)
			{
				reachableStates.insert(destinationState);
			}
			if (!traversedStates.count(destinationState))
			{
				statesWave.push_back(destinationState);
			}
		}
		statesWave.pop_front();
	}

	auto stateIt = m_states.begin();
	while (stateIt != m_states.end())
	{
		if (!reachableStates.count(*stateIt))
		{
			m_transitionTable.erase(*stateIt);
			stateIt = m_states.erase(stateIt);
		}
		else
		{
			++stateIt;
		}
	}
}

