#pragma once
#include <map>
#include <vector>
#include <string>
#include <unordered_map>


using State = std::string;
using Signal = std::string;
using States = std::vector<State>;
using InputSignals = std::vector<Signal>;
using TransitionTable = std::unordered_map<State, States>;


class MealyTable
{
public:
	States GetStates() const;
	InputSignals GetInputSignals() const;
	TransitionTable GetTransitionTable() const;

	void SetStates(const States&);
	void SetInputSignals(const InputSignals&);
	void SetTransitionTable(const TransitionTable&);

	virtual ~MealyTable() = default;

protected:
	States m_states;
	InputSignals m_inputSignals;
	TransitionTable m_transitionTable;
};