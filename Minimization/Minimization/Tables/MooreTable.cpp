#include <deque>
#include <set>
#include "MooreTable.h"

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

	size_t equivalence—lass = 0;
	for (auto& outputSignal : m_outputSignals)
	{
		auto insertResult = equivalenceClasses.emplace(outputSignal, equivalence—lass);

		if (insertResult.second)
		{
			++equivalence—lass;
		}
	}

	for (auto& [state, outputSignal] : m_mooreStates)
	{
		m_eqvClasses[state] = equivalenceClasses[outputSignal];
	}

	size_t prevEqvClassesCount = equivalenceClasses.size();
	size_t currEqvClassesCount = 0;
	while (prevEqvClassesCount != currEqvClassesCount)
	{
		prevEqvClassesCount = currEqvClassesCount;
		currEqvClassesCount = CommonMinimize();
	}

	SetupTransitionTableByEquivalenceClasses();

	m_outputSignals.clear();
	for (auto& [oldState, outputSignal] : m_mooreStates)
	{
		if (m_eqvClasses.count(oldState))
		{
			m_outputSignals.push_back(outputSignal);
		}
	}
}
