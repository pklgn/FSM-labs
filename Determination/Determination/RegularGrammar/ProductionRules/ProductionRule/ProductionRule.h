#pragma once
#include "RightPart/RightPart.h"
namespace RegularGrammar
{
struct ProductionRule
{
	using RightParts = std::vector<RightPart>;

	NonTerminal leftPart;
	RightParts rightParts;
};
}; // namespace RegularGrammar