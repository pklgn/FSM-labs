#pragma once
#include "../../pch.h"

namespace RegularGrammar
{
using Terminal = char;
using Terminals = std::set<Terminal>;
using NonTerminal = char;
using NonTerminals = std::vector<NonTerminal>;

const Terminal NO_TERMINAL = ' ';
const Terminal NO_NONTERMINAL = ' ';
} // namespace Grammar