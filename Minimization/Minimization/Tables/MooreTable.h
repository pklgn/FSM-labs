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
			auto stateIndex = states.size() - 1;
			auto mStateIndex = m_states.size() - 1;
			while (stateIndex > 0)
			{
				if (states[stateIndex] != m_states[mStateIndex])
				{
					m_outputSignals.erase(m_outputSignals.begin() + stateIndex);
					--stateIndex;
				}
				else
				{
					--stateIndex;
					--mStateIndex;
				}
			}
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

protected:
	MooreStateTransitions m_mooreStates;
	Signals m_outputSignals;
};