#pragma once
#include <algorithm>
#include <iterator>
#include "FSMTable.hpp"

using stateOutputSignal = std::unordered_map<State, Signal>;

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
			std::inserter(m_stateOutputSignal, m_stateOutputSignal.begin()),
			[](const auto& state, const auto& outputSignal) {
				return std::pair<State, Signal>(state, outputSignal);
			});
	}

	Signals GetOutputSignals() const;

	void Minimize() override;

	void RenameStates(const std::string& prefix = "S");

	MooreTable Determine();

protected:
	std::set<State> GetEClosure(const State& state, std::set<State>& closures);
	std::set<State> GetEClosures(const std::set<State>& states, std::set<State>& closures);
	void AppendOutputSignal(Signals&, const std::set<State>& currStates);
	void RemoveUnreachableOutputSignals(const States&);

	Signals m_outputSignals;
	stateOutputSignal m_stateOutputSignal;
};