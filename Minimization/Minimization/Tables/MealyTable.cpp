#include <deque>
#include <set>
#include <unordered_set>
#include <algorithm>
#include "MealyTable.h"

void MealyTable::Minimize()
{
	std::unordered_map<Signals, EquivalenceClass, FSMHashFunction> equivalenceClasses;

	size_t equivalenceClass = 0;
	for (auto& [_, transitions] : m_transitionTable)
	{
		auto insertResult = equivalenceClasses.emplace(transitions.outputSignals, equivalenceClass);

		if (insertResult.second)
		{
			++equivalenceClass;
		}
	}

	for (auto& [srcState, transitions] : m_transitionTable)
	{
		m_eqvClasses[srcState] = equivalenceClasses[transitions.outputSignals];
	}

	size_t prevEqvClassesCount = equivalenceClasses.size();
	size_t currEqvClassesCount = 0;
	while (prevEqvClassesCount != currEqvClassesCount)
	{
		prevEqvClassesCount = currEqvClassesCount;
		currEqvClassesCount = CommonMinimize();
	}

	SetupTransitionTableByEquivalenceClasses();
}

