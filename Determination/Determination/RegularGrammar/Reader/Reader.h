#pragma once
#include <istream>
#include "../common/GrammarTypes.h"
#include "../ProductionRules/ProductionRules.h"

namespace RegularGrammar
{
class Reader
{
public:
	Reader(GrammarType, std::istream&);

	ProductionRules ReadProductionRules();

private:
	void ReadProductionRuleRightParts(ProductionRule::RightParts&);
	GrammarType m_grammarType;
	std::istream& m_inputStream;
};
} // namespace RegularGrammar