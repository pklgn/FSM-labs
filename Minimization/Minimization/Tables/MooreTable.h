#pragma once
#include <algorithm>
#include <iterator>
#include "FSMTable.hpp"

using OutputSignals = std::vector<Signal>;
using MooreState = StateTransition;
using MooreStates = std::vector<MooreState>;
using MooreTransitionTable = std::unordered_map<State, States>;

class MooreTable : public FSMTable<MooreTransitionTable>
{
public:
	MooreTable(States& states, InputSignals& inputSignals, MooreTransitionTable& transitionTable, OutputSignals& outputSignals)
		: FSMTable(states, inputSignals, transitionTable)
		, m_outputSignals(outputSignals)
	{
		RemoveUnreachableStates();

		std::transform(m_states.begin(), m_states.end(), m_outputSignals.begin(),
			std::back_inserter(m_mooreStates),
			[](const auto& state, const auto& outputSignal) {
				return StateTransition{ state, outputSignal };
			});
	}

	OutputSignals GetOutputSignals() const;
	MooreStates GetMooreStates() const;

	void Minimize() override;

protected:
	void RemoveUnreachableStates() override;

	void RecursiveMinimize() override;

	MooreStates m_mooreStates;
	OutputSignals m_outputSignals;
};