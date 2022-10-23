#pragma once
#include <map>
#include <set>
#include <deque>
#include <vector>
#include <string>
#include <algorithm>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>

using State = std::string;
using Signal = std::string;
using States = std::vector<State>;
using Signals = std::vector<Signal>;

using Equivalence—lass = size_t;
using StateEquivalence—lasses = std::unordered_map<State, Equivalence—lass>;

struct FSMStateTransitions
{
	States commonStates;
	States aliasedStates;
};

struct MealyStateTransitions : FSMStateTransitions
{
	Signals outputSignals;
};

using MealyTransitionTable = std::unordered_map<State, MealyStateTransitions>;
using MooreTransitionTable = std::unordered_map<State, FSMStateTransitions>;

template <typename T>
class FSMTable
{
public:
	FSMTable(States& states, Signals& inputSignals, T& transitionTable)
		: m_states(states)
		, m_inputSignals(inputSignals)
		, m_transitionTable(transitionTable)
	{
		RemoveUnreachableStates();

		for (auto& [_, transitions] : m_transitionTable)
		{
			transitions.aliasedStates.resize(m_inputSignals.size());
		}
	}

	States GetStates() const
	{
		return m_states;
	}

	Signals GetInputSignals() const
	{
		return m_inputSignals;
	}

	T GetTransitionTable() const
	{
		return m_transitionTable;
	}

	void RemoveUnreachableStates();

	virtual void Minimize() = 0;

	virtual ~FSMTable() = default;

protected:
	size_t CommonMinimize();
	void SetupTransitionTableByEquivalenceClasses();
	
	States m_states;
	Signals m_inputSignals;
	T m_transitionTable;
	StateEquivalence—lasses m_eqvClasses;
};

struct FSMHashFunction
{
	size_t operator()(const Signals& signals) const
	{
		std::hash<Signal> hasher;
		size_t answer = 0;

		for (Signal i : signals)
		{
			answer ^= hasher(i) + 0x9e3779b9 + (answer << 6) + (answer >> 2);
		}

		return answer;
	}
};

template <typename T>
inline void FSMTable<T>::RemoveUnreachableStates()
{
	auto inspectedState = m_states.front();

	std::set<State> traversedStates;
	std::set<State> reachableStates;
	std::deque<State> statesWave;

	reachableStates.insert(inspectedState);

	for (auto& state : m_transitionTable[inspectedState].commonStates)
	{
		if (inspectedState != state)
		{
			statesWave.push_back(state);
			reachableStates.insert(state);
		}
	}
	traversedStates.insert(inspectedState);

	while (!statesWave.empty())
	{
		inspectedState = statesWave.front();
		traversedStates.insert(inspectedState);

		for (auto& transitionState : m_transitionTable[inspectedState].commonStates)
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
		}
		else
		{
			++stateIt;
		}
	}
}

template <typename T>
inline size_t FSMTable<T>::CommonMinimize()
{
	//new
	std::unordered_map<Equivalence—lass, std::set<State>> eqvivalenceSets;
	for (auto& [state, eqvClass] : m_eqvClasses)
	{
		eqvivalenceSets[eqvClass].insert(state);
	}

	// assign aliases to all transitions column-by-column with defined equivalence classes
	for (auto& [_, transitions] : m_transitionTable)
	{
		for (size_t inputSignal = 0; inputSignal < m_inputSignals.size(); ++inputSignal)
		{
			transitions.aliasedStates[inputSignal] = std::to_string(m_eqvClasses[transitions.commonStates[inputSignal]]);
		}
	}

	// form new equivalence classes by states
	std::unordered_map<States, Equivalence—lass, FSMHashFunction> equivalence—lasses;
	Equivalence—lass equivalence—lass = 0;
	for (auto& [srcState, transitions] : m_transitionTable)
	{
		auto insertResult = equivalence—lasses.emplace(transitions.aliasedStates, equivalence—lass);

		if (insertResult.second)
		{
			++equivalence—lass;
		}
	}

	// save all states with a new equivalence class alias
	for (auto& [srcState, transitions] : m_transitionTable)
	{
		m_eqvClasses[srcState] = equivalence—lasses[transitions.aliasedStates];
	}

	return equivalence—lasses.size();
}

template <typename T>
inline void FSMTable<T>::SetupTransitionTableByEquivalenceClasses()
{
	// remain only one initial state from each equivalence set
	std::unordered_set<Equivalence—lass> uniqueEqvClasses;
	for (auto eqvClassesIt = m_eqvClasses.begin(); eqvClassesIt != m_eqvClasses.end();)
	{
		auto insertResult = uniqueEqvClasses.insert(eqvClassesIt->second);
		if (!insertResult.second)
		{
			eqvClassesIt = m_eqvClasses.erase(eqvClassesIt);
		}
		else
		{
			++eqvClassesIt;
		}
	}

	// remove extra states from transition table
	for (auto stateIt = m_transitionTable.begin(); stateIt != m_transitionTable.end();)
	{
		if (!m_eqvClasses.count(stateIt->first))
		{
			stateIt = m_transitionTable.erase(stateIt);
		}
		else
		{
			++stateIt;
		}
	}

	// set transition table with new equivalence class state name and remove old states
	for (auto& [oldState, aliasedState] : m_eqvClasses)
	{
		auto newState = std::to_string(aliasedState);
		m_transitionTable.emplace(newState, m_transitionTable[oldState]);
		m_transitionTable[newState].commonStates = m_transitionTable[oldState].aliasedStates;
		m_transitionTable.erase(oldState);
	}

	m_states.clear();
	std::for_each(uniqueEqvClasses.begin(), uniqueEqvClasses.end(), [this](const Equivalence—lass& eqvClass) {
		m_states.push_back(std::to_string(eqvClass));
	});
}
