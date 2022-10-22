#include <deque>
#include <set>
#include <unordered_set>
#include <algorithm>
#include "MealyTable.h"

struct HashFunction
{
	size_t operator()(const std::vector<std::string>& myVector) const
	{
		std::hash<std::string> hasher;
		size_t answer = 0;

		for (std::string i : myVector)
		{
			answer ^= hasher(i) + 0x9e3779b9 + (answer << 6) + (answer >> 2);
		}
		return answer;
	}
};

void MealyTable::Minimize()
{
	std::unordered_set<std::vector<Signal>, HashFunction> uniqueOutputSignals;

	for (auto& column : m_transitionTable)
	{
		auto insertResult = uniqueOutputSignals.insert(column.second.outputSignals);
		m_eqvClasses[column.first] = uniqueOutputSignals.size();
	}

	RecursiveMinimize(uniqueOutputSignals.size());
}

void MealyTable::RecursiveMinimize(size_t eqvClassesCount)
{
	for (auto& column : m_transitionTable)
	{
		for (size_t rowIndex = 0; rowIndex < m_inputSignals.size(); ++rowIndex)
		{
			column.second.renamedStates[rowIndex] = column.second.states[rowIndex];
		}
	}

	m_eqvClasses.clear();

	std::unordered_set<States, HashFunction> uniqueStates;
	for (auto& column : m_transitionTable)
	{
		auto insertResult = uniqueStates.insert(column.second.renamedStates);
		m_eqvClasses[column.first] = uniqueStates.size();
	}

	if (uniqueStates.size() != eqvClassesCount)
	{
		RecursiveMinimize(uniqueStates.size());
	}

	return;
}

void MealyTable::RemoveUnreachableStates()
{
	State inspectedState = m_states.front();

	std::set<State> traversedStates;
	std::set<State> reachableStates;
	std::deque<State> statesWave;

	reachableStates.insert(inspectedState);
	for (auto& mealyState : m_transitionTable[inspectedState].states)
	{
		if (inspectedState != mealyState)
		{
			statesWave.push_back(mealyState);
			reachableStates.insert(mealyState);
		}
	}
	traversedStates.insert(inspectedState);

	while (!statesWave.empty())
	{
		inspectedState = statesWave.front();
		traversedStates.insert(inspectedState);
		for (auto& transitionState : m_transitionTable[inspectedState].states)
		{
			State destinationState = transitionState;
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

