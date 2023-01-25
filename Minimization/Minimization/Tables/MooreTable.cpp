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

	size_t equivalenceClass = 0;
	for (auto& outputSignal : m_outputSignals)
	{
		auto insertResult = equivalenceClasses.emplace(outputSignal, equivalenceClass);

		// если выходной сигнал не был вставлен в множество, значит он там уже есть
		// в противном случае это означает, что мы нашли новый класс эквивалентности
		if (insertResult.second)
		{
			++equivalenceClass;
		}
	}

	// записываем к какому классу эквивалентности на данный момент принадлежит каждое состояние
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
