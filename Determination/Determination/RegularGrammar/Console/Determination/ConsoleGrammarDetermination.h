#pragma once
#include <string>
#include "../../ProductionRules/ProductionRule/RightPart/RightPart.h"

using namespace RegularGrammar;

class ConsoleGrammarDetermination
{
public:
	ConsoleGrammarDetermination(int argc, char* argv[]);

	void Run() const;

private:
	GrammarType GetGrammarTypeMode(const std::string& modeStr);

	GrammarType m_grammarType;
	std::string m_inputFileName;
	std::string m_outputFileName;
};
