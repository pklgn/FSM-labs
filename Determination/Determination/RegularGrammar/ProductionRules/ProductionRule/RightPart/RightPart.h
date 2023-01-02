#pragma once
#include "../../../common/GrammarTypes.h"

namespace RegularGrammar
{
enum class GrammarType
{
	LeftSide,
	RightSide,
};

using RawRightPart = std::string;
struct RightPart
{
	RightPart(GrammarType, const RawRightPart&);

	Terminal terminal = NO_TERMINAL;
	NonTerminal nonTerminal = NO_NONTERMINAL;
};
} // namespace RegularGrammar