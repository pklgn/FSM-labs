#include "../../pch.h"
#include "ProductionRules.h"

namespace RegularGrammar
{
const NonTerminal INITIAL_STATE = 'H';
const Signal FINISHING_SIGNAL = "F";
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

using SrcTransitionStates = std::unordered_set<NonTerminal>;
void AppendOutputSignal(Signals& outputSignals, NonTerminal finishingState, const SrcTransitionStates& srcStates)
{
	if (srcStates.contains(finishingState))
	{
		outputSignals.push_back(FINISHING_SIGNAL);
		return;
	}
	
	outputSignals.push_back(NO_SIGNAL);
}

MooreTable ProductionRules::DetermineLeftSide() const
{
	std::deque<SrcTransitionStates> statesToDetermine;
	statesToDetermine.push_back({ NO_NONTERMINAL });

	using DstTransitionStates = std::unordered_set<NonTerminal>;
	std::unordered_map<Terminal, DstTransitionStates> dstStates;
	for (auto&& terminal : m_terminals)
	{
		dstStates[terminal];
	}

	States states;
	Signals outputSignals;
	MooreTransitionTable transitionTable;
	std::unordered_set<State> viewedStates;
	auto finishingState = m_nonTerminals.front();
	while (!statesToDetermine.empty())
	{
		SrcTransitionStates srcStates = statesToDetermine.front();
		viewedStates.insert(Join<SrcTransitionStates::const_iterator>(srcStates.begin(), srcStates.end()));
		AppendOutputSignal(outputSignals, finishingState, srcStates);

		for (auto&& [leftPart, rightParts] : m_rules)
		{
			for (auto&& rightPart : rightParts)
			{
				if (srcStates.contains(rightPart.nonTerminal))
				{
					dstStates[rightPart.terminal].insert(leftPart);
				}
			}
		}

		FSMStateTransitions stateTransitions;
		std::for_each(m_terminals.begin(), m_terminals.end(), [&](Terminal terminal) {
			auto& transitions = dstStates[terminal];
			auto transitionsStr = Join<SrcTransitionStates::const_iterator>(transitions.begin(), transitions.end());
			stateTransitions.commonStates.push_back(transitionsStr);
			if (!transitionsStr.empty() &&
				!viewedStates.contains(transitionsStr))
			{
				statesToDetermine.push_back(transitions);
			}
		});
		State state = Join<SrcTransitionStates::const_iterator>(srcStates.begin(), srcStates.end());
		transitionTable.emplace(state, stateTransitions);
		states.push_back(state);

		statesToDetermine.pop_front();

		for (auto&& terminal : dstStates)
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
	std::deque<SrcTransitionStates> statesToDetermine;
	statesToDetermine.push_back({ m_nonTerminals.front() });

	using DstTransitionStates = std::unordered_set<NonTerminal>;
	std::unordered_map<Terminal, DstTransitionStates> dstStates;
	for (auto&& terminal : m_terminals)
	{
		dstStates[terminal];
	}

	States states;
	Signals outputSignals;
	MooreTransitionTable transitionTable;
	std::unordered_set<State> viewedStates;
	auto finishingState = INITIAL_STATE;
	while (!statesToDetermine.empty())
	{
		SrcTransitionStates srcStates = statesToDetermine.front();
		viewedStates.insert(Join<SrcTransitionStates::const_iterator>(srcStates.begin(), srcStates.end()));
		AppendOutputSignal(outputSignals, finishingState, srcStates);

		for (auto&& [leftPart, rightParts] : m_rules)
		{
			for (auto&& rightPart : rightParts)
			{
				if (srcStates.contains(leftPart))
				{
					auto dstState = (rightPart.nonTerminal == NO_NONTERMINAL)
						? INITIAL_STATE
						: rightPart.nonTerminal;
					dstStates[rightPart.terminal].insert(dstState);
				}
			}
		}

		FSMStateTransitions stateTransitions;
		std::for_each(m_terminals.begin(), m_terminals.end(), [&](Terminal terminal) {
			auto& transitions = dstStates[terminal];
			auto transitionsStr = Join<SrcTransitionStates::const_iterator>(transitions.begin(), transitions.end());
			std::sort(transitionsStr.begin(), transitionsStr.end());
			stateTransitions.commonStates.push_back(transitionsStr);
			if (!transitionsStr.empty() && !viewedStates.contains(transitionsStr))
			{
				statesToDetermine.push_back(transitions);
			}
			viewedStates.insert(transitionsStr);
		});
		State state = Join<SrcTransitionStates::const_iterator>(srcStates.begin(), srcStates.end());
		std::sort(state.begin(), state.end());
		transitionTable.emplace(state, stateTransitions);
		states.push_back(state);

		statesToDetermine.pop_front();

		for (auto&& terminal : dstStates)
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