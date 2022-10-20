#include <deque>
#include <set>
#include "MooreTable.h"

OutputSignals MooreTable::GetOutputSignals() const
{
	return m_outputSignals;
}

MooreStates MooreTable::GetMooreStates() const
{
	return m_mooreStates;
}

void MooreTable::Minimize()
{
}

void MooreTable::RemoveUnreachableStates()
{
	State inspectedState = m_states.front();

	std::set<State> traversedStates;
	std::set<State> reachableStates;
	std::deque<State> statesWave;

	reachableStates.insert(inspectedState);

	for (auto& transitionState : m_transitionTable[inspectedState])
	{
		if (inspectedState != transitionState)
		{
			statesWave.push_back(transitionState);
			reachableStates.insert(transitionState);
		}
	}
	traversedStates.insert(inspectedState);

	while (!statesWave.empty())
	{
		inspectedState = statesWave.front();
		traversedStates.insert(inspectedState);
		for (auto& transitionState : m_transitionTable[inspectedState])
		{
			if (inspectedState != transitionState)
			{
				reachableStates.insert(transitionState);
			}
			if (!traversedStates.count(transitionState))
			{
				statesWave.push_back(transitionState);
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
			m_outputSignals.erase(m_outputSignals.begin() + std::distance(m_states.begin(), stateIt));
		}
		else
		{
			++stateIt;
		}
	}
}

void MooreTable::RecursiveMinimize()
{
}
