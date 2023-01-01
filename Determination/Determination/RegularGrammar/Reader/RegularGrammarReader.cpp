#include "../../pch.h"
#include "RegularGrammarReader.h"

namespace RegularGrammar
{

const std::string INCOMPLETE_LEFT_PART_MESSAGE = "Incomplete left part of the product rule was found";
const std::string INCOMPLETE_RIGHT_PART_MESSAGE = "Incomplete right part of the product rule was found";
const std::string RULE_PARTS_SEPARATOR = "->";
const char RULE_RIGHT_PARTS_SEPARATOR = '|';

Reader::Reader(std::istream& input)
	: m_inputStream(input)
{
}

ProductionRules Reader::ReadProductionRules()
{
	ProductionRules productionRules;

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

	return productionRules;
}

using RawRightParts = std::string;
void Reader::ReadProductionRuleRightParts(ProductionRule::RightParts& rightParts)
{
	RawRightParts rawRightParts;
	std::getline(m_inputStream, rawRightParts);

	size_t last = 0;
	size_t next = 0;
	ProductionRule::RightPart rightPart;
	while ((next = rawRightParts.find(RULE_RIGHT_PARTS_SEPARATOR)) != std::string::npos)
	{
		rightPart = rawRightParts.substr(last, next - last);
		rightParts.push_back(rightPart);
		last = next + 1;
	}
	rightPart = rawRightParts.substr(last);
	if (rightPart == "")
	{
		std::cout << INCOMPLETE_RIGHT_PART_MESSAGE << '\n';
		return;
	}

	rightParts.push_back(rightPart);
}
} // namespace RegularGrammar