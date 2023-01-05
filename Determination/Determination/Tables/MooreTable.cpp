#include "../pch.h"
#include "MooreTable.h"

const State NO_STATE = "";
const State INITIAL_STATE = "H";
const State NO_STATE_VISIBLE = "-";
const Signal EPSILON_SIGNAL = "e";
const Signal FINISHING_SIGNAL = "F";

Signals MooreTable::GetOutputSignals() const
{
	return m_outputSignals;
}

void MooreTable::Minimize()
{
	std::unordered_map<Signal, EquivalenceClass> equivalenceClasses;

	size_t equivalenceClass = 0;
	for (auto& outputSignal : m_outputSignals)
	{
		auto insertResult = equivalenceClasses.emplace(outputSignal, equivalenceClass);

		if (insertResult.second)
		{
			++equivalenceClass;
		}
	}

	for (auto& [state, outputSignal] : m_stateOutputSignal)
	{
		m_eqvClasses[state] = equivalenceClasses[outputSignal];
	}

	CompleteCommonMinimization(equivalenceClasses.size());

	m_outputSignals.clear();
	for (auto& [oldState, outputSignal] : m_stateOutputSignal)
	{
		if (m_eqvClasses.count(oldState))
		{
			m_outputSignals.push_back(outputSignal);
		}
	}
}

void MooreTable::RenameStates(const std::string& prefix)
{
	std::unordered_map<State, State> renamedStates;
	size_t index = 0;
	for (auto&& state : m_states)
	{
		if (state == " ")
		{
			renamedStates.emplace(state, INITIAL_STATE);
			std::cout << "Added initial state "
					  << renamedStates[state] << '\n';
		}
		else
		{
			renamedStates.emplace(state, prefix + std::to_string(index));
			std::cout << renamedStates[state]
					  << " = "
					  << state << '\n';
			++index;
		}
		state = renamedStates[state];
	}

	MooreTransitionTable renamedTransitionTable;
	for (auto&& [srcState, transitions] : m_transitionTable)
	{
		for (auto&& states : transitions.commonStates)
		{
			for (auto&& state : states)
			{
				if (state == NO_STATE)
				{
					state = NO_STATE_VISIBLE;
				}
				else
				{
					state = renamedStates[state];
				}
			}
		}
		renamedTransitionTable[renamedStates[srcState]] = transitions;
	}
	m_transitionTable = renamedTransitionTable;
}

std::set<State> MooreTable::GetEClosure(const State& state)
{
	std::set<State> resultSet;
	if (state == NO_STATE)
	{
		return resultSet;
	}
	// assumed that the table will always have a line specifying the epsilon transitions at the end of the table
	auto eIndex = m_inputSignals.size() - 1;
	States closures;
	auto closureStates = m_transitionTable[state].commonStates[eIndex];
	std::move(closureStates.begin(), closureStates.end(), std::inserter(resultSet, resultSet.begin()));
	std::for_each(resultSet.begin(), resultSet.end(), [&](const State& state) {
		auto closure = GetEClosure(state);
		std::move(closure.begin(), closure.end(), std::back_inserter(closures));
	});
	std::move(closures.begin(), closures.end(), std::inserter(resultSet, resultSet.begin()));
	resultSet.insert(state);
	return resultSet;
}

std::set<State> MooreTable::GetEClosures(const std::set<State>& states)
{
	std::set<State> reachableStates;
	std::set<State> nextStates;
	for (auto&& state : states)
	{
		nextStates = GetEClosure(state);
		reachableStates.insert(nextStates.begin(), nextStates.end());
	}
	return reachableStates;
}

MooreTable MooreTable::Determine()
{
	States tableStates;
	MooreTransitionTable transitionTable;
	Signals outputSignals;

	bool haveEClosure = false;
	auto eIt = std::find(m_inputSignals.begin(), m_inputSignals.end(), EPSILON_SIGNAL);
	if (eIt != m_inputSignals.end())
	{
		haveEClosure = true;
	}

	std::unordered_set<States, FSMHashFunction> states;
	std::deque<std::set<State>> statesToDetermine;
	std::set<State> currStates;
	std::set<State> nextStates;
	statesToDetermine.push_back({ m_states.front() });
	while (!statesToDetermine.empty())
	{
		currStates = statesToDetermine.front();
		if (haveEClosure)
		{
			currStates = GetEClosures(currStates);
		}
		States currStatesVec = { currStates.begin(), currStates.end() };
		states.insert(currStatesVec);
		tableStates.push_back(std::accumulate(currStates.begin(), currStates.end(), std::string("")));
		// output signal
		bool isFinishing = false;
		for (auto&& currState : currStates)
		{
			if (m_stateOutputSignal[currState] == FINISHING_SIGNAL)
			{
				isFinishing = true;
				break;
			}
		}
		if (isFinishing)
		{
			outputSignals.push_back(FINISHING_SIGNAL);
		}
		else
		{
			outputSignals.push_back("");
		}
		FSMStateTransitions currTransitions;
		auto signalsSize = m_inputSignals.size();
		if (haveEClosure)
		{
			--signalsSize;
		}
		currTransitions.commonStates.resize(signalsSize);
		for (size_t signalIndex = 0; signalIndex < m_inputSignals.size(); ++signalIndex)
		{
			if (m_inputSignals[signalIndex] != EPSILON_SIGNAL)
			{
				for (auto&& currState : currStates)
				{
					auto dstStates = m_transitionTable[currState].commonStates[signalIndex];
					nextStates.insert(dstStates.begin(), dstStates.end());
				}
				auto nextStatesEClosure = (haveEClosure)
					? GetEClosures(nextStates)
					: nextStates;
				States nextStatesVec = { nextStatesEClosure.begin(), nextStatesEClosure.end() };
				if (!states.contains(nextStatesVec))
				{
					statesToDetermine.push_back(nextStates);
					states.insert(nextStatesVec);
				}
				if (!nextStates.empty())
				{
					currTransitions.commonStates[signalIndex] = { std::accumulate(nextStatesEClosure.begin(), nextStatesEClosure.end(), std::string("")) };
				}
				nextStates.clear();
			}
		}
		transitionTable[tableStates.back()] = currTransitions;
		statesToDetermine.pop_front();
	}
	Signals inputSignals = m_inputSignals;
	if (haveEClosure)
	{
		auto it = std::remove_if(inputSignals.begin(), inputSignals.end(), [](const Signal& signal) {
			return signal == EPSILON_SIGNAL;
		});
		inputSignals.erase(it, inputSignals.end());
	}

	return MooreTable{ tableStates, inputSignals, transitionTable, outputSignals };
}

void MooreTable::RemoveUnreachableOutputSignals(const States& oldStates)
{
	/*
	* passing through the elements from the end because we will always 
	* have a not deleted first state
	*/
	auto oldStateIndex = oldStates.size() - 1;
	auto currStateIndex = m_states.size() - 1;
	while (oldStateIndex > 0)
	{
		if (oldStates[oldStateIndex] != m_states[currStateIndex])
		{
			m_outputSignals.erase(m_outputSignals.begin() + oldStateIndex);
			--oldStateIndex;
		}
		else
		{
			--oldStateIndex;
			--currStateIndex;
		}
	}
}
