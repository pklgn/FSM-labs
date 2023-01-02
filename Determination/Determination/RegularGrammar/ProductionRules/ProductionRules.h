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
	MooreTable DetermineLeftSide() const;
	MooreTable DetermineRightSide() const;

	GrammarType m_grammarType;
	Terminals m_terminals;
	NonTerminals m_nonTerminals;
	std::vector<ProductionRule> m_rules;
};
} // namespace RegularGrammar