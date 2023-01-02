#include "ProductionRules.h"

namespace RegularGrammar
{

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
		// TUDU: добавить начальное состояние H
		// TUDU: дальше ищем все переходы только с терминалом в правой части
		// TUDU: после того как заполнили все переходы в строке - добавляем их по порядку терминалов в очередь и начинаем заново цикл

		break;
	case GrammarType::RightSide:
		// TUDU: добавлять ничего не нужно 
		break;
	}
}

MooreTable ProductionRules::DetermineLeftSide() const
{
	// добавляем начальное состояние
}
} // namespace RegularGrammar