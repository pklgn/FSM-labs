#pragma once
#include <istream>
#include "../common/GrammarTypes.h"

namespace RegularGrammar
{
class Reader
{
public:
	Reader(std::istream&);

	ProductionRules ReadProductionRules();

private:
	void ReadProductionRuleRightParts(ProductionRule::RightParts&);
	std::istream& m_inputStream;
};
} // namespace RegularGrammar