#include <deque>
#include <set>
#include <unordered_set>
#include <algorithm>
#include "MealyTable.h"

void MealyTable::Minimize()
{
	std::unordered_map<Signals, Equivalence—lass, FSMHashFunction> equivalence—lasses;

	size_t equivalence—lass = 0;
	for (auto& [_, transitions] : m_transitionTable)
	{
		auto insertResult = equivalence—lasses.emplace(transitions.outputSignals, equivalence—lass);

		if (insertResult.second)
		{
			++equivalence—lass;
		}
	}

	for (auto& [srcState, transitions] : m_transitionTable)
	{
		m_eqvClasses[srcState] = equivalence—lasses[transitions.outputSignals];
	}

	size_t prevEqvClassesCount = equivalence—lasses.size();
	size_t currEqvClassesCount = 0;
	while (prevEqvClassesCount != currEqvClassesCount)
	{
		prevEqvClassesCount = currEqvClassesCount;
		currEqvClassesCount = CommonMinimize();
	}

	SetupTransitionTableByEquivalenceClasses();
}

