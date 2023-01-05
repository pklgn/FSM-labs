#include "../../pch.h"
#include "Reader.h"
#include "../ProductionRules/ProductionRules.h"

namespace RegularGrammar
{

const std::string INCOMPLETE_LEFT_PART_MESSAGE = "Incomplete left part of the product rule was found";
const std::string INCOMPLETE_RIGHT_PART_MESSAGE = "Incomplete right part of the product rule was found";
const std::string RULE_PARTS_SEPARATOR = "->";
const char RULE_RIGHT_PARTS_SEPARATOR = '|';

Reader::Reader(GrammarType type, std::istream& input)
	: m_inputStream(input)
	, m_grammarType(type)
{
}

ProductionRules Reader::ReadProductionRules()
{
	std::vector<ProductionRule> productionRules;

	std::string ruleSeparator;
	while (m_inputStream.good())
	{
		ProductionRule rule;
		m_inputStream >> rule.leftPart;
		m_inputStream >> ruleSeparator;

		if (ruleSeparator != RULE_PARTS_SEPARATOR)
		{
			std::cout << INCOMPLETE_LEFT_PART_MESSAGE << '\n';
			std::cout << "Production rule will be ignored\n";
			break;
		}

		ReadProductionRuleRightParts(rule.rightParts);
		
		productionRules.push_back(rule);
	}

	return ProductionRules(m_grammarType, productionRules);
}

void Reader::ReadProductionRuleRightParts(ProductionRule::RightParts& rightParts)
{
	using RawRightParts = std::string;
	RawRightParts rawRightParts;
	std::getline(m_inputStream, rawRightParts);

	size_t last = 0;
	size_t next = 0;
	RawRightPart rawRightPart;
	while ((next = rawRightParts.find(RULE_RIGHT_PARTS_SEPARATOR, last)) != std::string::npos)
	{
		rawRightPart = Trim(rawRightParts.substr(last, next - last));

		rightParts.push_back(RightPart(m_grammarType, rawRightPart));
		last = next + 1;
	}
	rawRightPart = Trim(rawRightParts.substr(last));
	if (rawRightPart == "")
	{
		std::cout << INCOMPLETE_RIGHT_PART_MESSAGE << '\n';
		return;
	}

	rightParts.push_back(RightPart(m_grammarType, rawRightPart));
}
} // namespace RegularGrammar