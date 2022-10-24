#pragma once
#include <map>
#include <set>
#include <deque>
#include <vector>
#include <string>
#include <optional>
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

struct SourceStateEquivalence
{
	State srcState;
	Equivalence—lass eqvClass;
};
using SourceStatesEquivalence = std::unordered_multimap<States, SourceStateEquivalence, FSMHashFunction>;

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
	SourceStatesEquivalence::const_iterator CheckForEquivalence(const SourceStatesEquivalence&, const FSMStateTransitions&, const State&);
	
	States m_states;
	Signals m_inputSignals;
	T m_transitionTable;
	StateEquivalence—lasses m_eqvClasses;
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
	// assign aliases to all transitions column-by-column with defined equivalence classes
	for (auto& [_, transitions] : m_transitionTable)
	{
		for (size_t inputSignal = 0; inputSignal < m_inputSignals.size(); ++inputSignal)
		{
			transitions.aliasedStates[inputSignal] = std::to_string(m_eqvClasses[transitions.commonStates[inputSignal]]);
		}
	}

	// form new equivalence classes by states
	SourceStatesEquivalence statesEquivalence;
	Equivalence—lass equivalence—lass = 0;
	StateEquivalence—lasses newEqvClasses;
	for (auto& [srcState, transitions] : m_transitionTable)
	{
		SourceStatesEquivalence::const_iterator eqvClass = statesEquivalence.end();
		if (statesEquivalence.count(transitions.aliasedStates))
		{
			eqvClass = CheckForEquivalence(statesEquivalence, transitions, srcState);
		}

		if (eqvClass == statesEquivalence.end())
		{
			auto insertResult = statesEquivalence.emplace(transitions.aliasedStates, SourceStateEquivalence{ srcState, equivalence—lass });
			newEqvClasses[srcState] = equivalence—lass;
			++equivalence—lass;
		}
		else
		{
			newEqvClasses[srcState] = eqvClass->second.eqvClass;
		}
	}

	m_eqvClasses = newEqvClasses;

	return statesEquivalence.size();
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

template <typename T>
inline SourceStatesEquivalence::const_iterator FSMTable<T>::CheckForEquivalence(const SourceStatesEquivalence& statesEquivalence,
	const FSMStateTransitions& transitions,
	const State& srcState)
{
	auto range = statesEquivalence.equal_range(transitions.aliasedStates);
	for (auto it = range.first; it != range.second; ++it)
	{
		if (m_eqvClasses[it->second.srcState] == m_eqvClasses[srcState])
		{
			return it;
		}
	}

	return statesEquivalence.end();
}
