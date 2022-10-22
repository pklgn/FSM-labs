#pragma once
#include <map>
#include <vector>
#include <string>
#include <unordered_map>

using State = std::string;
using Signal = std::string;
using States = std::vector<State>;
using InputSignals = std::vector<Signal>;

using StateEquivalence—lasses = std::unordered_map<State, size_t>;

struct StateTransitions
{
	std::vector<State> states;
	std::vector<Signal> outputSignals;

	std::vector<State> renamedStates;
};

template <typename T>
class FSMTable
{
public:
	FSMTable(States& states, InputSignals& inputSignals, T& transitionTable)
		: m_states(states)
		, m_inputSignals(inputSignals)
		, m_transitionTable(transitionTable)
	{
	}

	States GetStates() const
	{
		return m_states;
	}

	InputSignals GetInputSignals() const
	{
		return m_inputSignals;
	}

	T GetTransitionTable() const
	{
		return m_transitionTable;
	}

	virtual void RemoveUnreachableStates() = 0;

	virtual void Minimize() = 0;

	virtual ~FSMTable() = default;

protected:
	virtual void RecursiveMinimize(size_t eqvClassesCount) = 0;

	States m_states;
	InputSignals m_inputSignals;
	T m_transitionTable;
	StateEquivalence—lasses m_eqvClasses;
};
