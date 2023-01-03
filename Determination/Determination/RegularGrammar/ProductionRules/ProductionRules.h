#pragma once
#include "../../pch.h"
#include "ProductionRule/ProductionRule.h"
#include "../../Tables/MooreTable.h"

namespace RegularGrammar
{
class ProductionRules
{
public:
	ProductionRules(GrammarType, const std::vector<ProductionRule>&);

	const Terminals& GetTerminals() const;
	const NonTerminals& GetNonTerminals() const;

	MooreTable Determine() const;

private:
	using SrcTransitionStates = std::unordered_set<NonTerminal>;
	using DstTransitionStates = std::unordered_set<NonTerminal>;
	SrcTransitionStates ChooseStartingStates() const;
	NonTerminal ChooseFinishingState() const;
	void FormLeftSideDstStates(const SrcTransitionStates& srcStates,
		std::unordered_map<Terminal, DstTransitionStates>& dstStatess) const;
	void FormRightSideDstStates(const SrcTransitionStates& srcStates,
		std::unordered_map<Terminal, DstTransitionStates>& dstStates) const;

	GrammarType m_grammarType;
	Terminals m_terminals;
	NonTerminals m_nonTerminals;
	std::vector<ProductionRule> m_rules;
};
} // namespace RegularGrammar