#include "../../pch.h"
#include "ProductionRules.h"

namespace RegularGrammar
{
const NonTerminal INITIAL_STATE = 'H';
const Signal FINISH_SIGNAL = "F";
const Signal NO_SIGNAL = "";

RegularGrammar::ProductionRules::ProductionRules(GrammarType type, const std::vector<ProductionRule>& rules)
	: m_rules(rules)
	, m_grammarType(type)
{
	for (auto&& [leftPart, rightParts] : m_rules)
	{
		m_nonTerminals.push_back(leftPart);
		for (auto&& rightPart : rightParts)
		{
			m_terminals.insert(rightPart.terminal);
		}
	}
}

const Terminals& RegularGrammar::ProductionRules::GetTerminals() const
{
	return m_terminals;
}

const NonTerminals& ProductionRules::GetNonTerminals() const
{
	return m_nonTerminals;
}

MooreTable ProductionRules::Determine() const
{
	switch (m_grammarType)
	{
	case GrammarType::LeftSide:
		return DetermineLeftSide();
	case GrammarType::RightSide:
		return DetermineRightSide();
	default:
		throw std::runtime_error("Unexpected grammar type was given");
	}
}

using SourceTransitionStates = std::unordered_set<NonTerminal>;
void AppendOutputSignal(Signals& outputSignals, NonTerminal finishNonTerminal, const SourceTransitionStates& transitions)
{
	if (transitions.contains(finishNonTerminal))
	{
		outputSignals.push_back(FINISH_SIGNAL);
	}
	else
	{
		outputSignals.push_back(NO_SIGNAL);
	}
}

MooreTable ProductionRules::DetermineLeftSide() const
{
	std::deque<SourceTransitionStates> nonTerminalsToDetermine;
	nonTerminalsToDetermine.push_back({ NO_NONTERMINAL });

	std::unordered_map<Terminal, SourceTransitionStates> terminalTransitions;
	for (auto&& terminal : m_terminals)
	{
		terminalTransitions[terminal];
	}

	States states;
	Signals outputSignals;
	MooreTransitionTable transitionTable;
	std::unordered_set<State> viewedStates;
	auto finishNonTerminal = m_nonTerminals.front();

	while (!nonTerminalsToDetermine.empty())
	{
		FSMStateTransitions nonTerminalTransitions;
		SourceTransitionStates nonTerminals = nonTerminalsToDetermine.front();
		viewedStates.insert(Join<SourceTransitionStates::const_iterator>(nonTerminals.begin(), nonTerminals.end()));

		AppendOutputSignal(outputSignals, finishNonTerminal, nonTerminals);

		for (auto&& [leftPart, rightParts] : m_rules)
		{
			for (auto&& rightPart : rightParts)
			{
				if (nonTerminals.contains(rightPart.nonTerminal))
				{
					terminalTransitions[rightPart.terminal].insert(leftPart);
				}
			}
		}
		std::for_each(m_terminals.begin(), m_terminals.end(), [&](Terminal terminal) {
			auto& transitions = terminalTransitions[terminal];
			auto transitionsString = Join<SourceTransitionStates::iterator>(transitions.begin(), transitions.end());
			nonTerminalTransitions.commonStates.push_back(transitionsString);
			if (!viewedStates.contains(transitionsString) && !transitionsString.empty())
			{
				nonTerminalsToDetermine.push_back(transitions);
			}
		});
		State state = Join<SourceTransitionStates::iterator>(nonTerminals.begin(), nonTerminals.end());
		transitionTable.emplace(state, nonTerminalTransitions);
		states.push_back(state);

		nonTerminalsToDetermine.pop_front();

		for (auto&& terminal : terminalTransitions)
		{
			terminal.second.clear();
		}
	}
	Signals inputSignals;
	std::transform(m_terminals.begin(), m_terminals.end(),
		std::back_inserter(inputSignals),
		[](char ch) { return std::string(1, ch); });
	return MooreTable{ states, inputSignals, transitionTable, outputSignals };
}

MooreTable ProductionRules::DetermineRightSide() const
{
	MooreTransitionTable transitionTable;
	using NonTerminalsTransitions = std::unordered_set<NonTerminal>;
	std::deque<NonTerminalsTransitions> nonTerminalsToDetermine;
	// add the initial state to start filling the table
	nonTerminalsToDetermine.push_back({ NO_NONTERMINAL });

	// init container to store dst nonTerminals transitions
	std::unordered_map<Terminal, NonTerminalsTransitions> terminalTransitions;
	for (auto&& terminal : m_terminals)
	{
		terminalTransitions[terminal];
	}

	std::unordered_set<State> viewedStates;
	Signals outputSignals;
	auto finishNonTerminal = m_nonTerminals.front();
	States states;

	while (!nonTerminalsToDetermine.empty())
	{
		FSMStateTransitions nonTerminalTransitions;
		NonTerminalsTransitions nonTerminals = nonTerminalsToDetermine.front();
		viewedStates.insert(Join<NonTerminalsTransitions::const_iterator>(nonTerminals.begin(), nonTerminals.end()));
		if (nonTerminals.contains(finishNonTerminal))
		{
			outputSignals.push_back(FINISH_SIGNAL);
		}
		else
		{
			outputSignals.push_back(NO_SIGNAL);
		}

		for (auto&& [leftPart, rightParts] : m_rules)
		{
			for (auto&& rightPart : rightParts)
			{
				if (nonTerminals.contains(rightPart.nonTerminal))
				{
					terminalTransitions[rightPart.terminal].insert(leftPart);
				}
			}
		}
		std::for_each(m_terminals.begin(), m_terminals.end(), [&](Terminal terminal) {
			auto& transitions = terminalTransitions[terminal];
			auto transitionsString = Join<NonTerminalsTransitions::iterator>(transitions.begin(), transitions.end());
			nonTerminalTransitions.commonStates.push_back(transitionsString);
			if (!viewedStates.contains(transitionsString) && !transitionsString.empty())
			{
				nonTerminalsToDetermine.push_back(transitions);
			}
		});
		State state = Join<NonTerminalsTransitions::iterator>(nonTerminals.begin(), nonTerminals.end());
		transitionTable.emplace(state, nonTerminalTransitions);
		states.push_back(state);

		nonTerminalsToDetermine.pop_front();

		for (auto&& terminal : terminalTransitions)
		{
			terminal.second.clear();
		}
	}
	Signals inputSignals;
	std::transform(m_terminals.begin(), m_terminals.end(),
		std::back_inserter(inputSignals),
		[](char ch) { return std::string(1, ch); });
	return MooreTable{ states, inputSignals, transitionTable, outputSignals };
}

} // namespace RegularGrammar