#pragma once
#include <algorithm>
#include <iterator>
#include "FSMTable.hpp"

struct MooreStateTransition
{
	State state;
	Signal outputSignal;
};

using MooreStateTransitions = std::vector<MooreStateTransition>;


class MooreTable : public FSMTable<MooreTransitionTable>
{
public:
	MooreTable(States& states, Signals& inputSignals, MooreTransitionTable& transitionTable, Signals& outputSignals)
		: FSMTable(states, inputSignals, transitionTable)
		, m_outputSignals(outputSignals)
	{
		if (states.size() != m_states.size())
		{
			RemoveUnreachableOutputSignals(states);
		}

		std::transform(m_states.begin(), m_states.end(), m_outputSignals.begin(),
			std::back_inserter(m_mooreStates),
			[](const auto& state, const auto& outputSignal) {
				return MooreStateTransition{ state, outputSignal };
			});
	}

	Signals GetOutputSignals() const;
	MooreStateTransitions GetMooreStates() const;

	void Minimize() override;

	void RenameStates(const std::string& prefix = "S");

protected:
	void RemoveUnreachableOutputSignals(const States&);

	MooreStateTransitions m_mooreStates;
	Signals m_outputSignals;
};