#pragma once
#include "../../pch.h"

namespace RegularGrammar
{
using Terminal = char;
using NonTerminal = char;
struct ProductionRule
{
	using RightPart = std::string;
	using RightParts = std::vector<RightPart>;

	NonTerminal leftPart;
	RightParts rightParts;
};
using ProductionRules = std::vector<ProductionRule>;
} // namespace Grammar