#include "../../../pch.h"
#include "ConsoleGrammarDetermination.h"
#include "../../Reader/Reader.h"
#include "../../../CSVWriter/CSVWriter.h"

ConsoleGrammarDetermination::ConsoleGrammarDetermination(int argc, char* argv[])
	: m_grammarType(GetGrammarTypeMode(argv[1]))
	, m_inputFileName(argv[2])
	, m_outputFileName(argv[3])
{
}

void ConsoleGrammarDetermination::Run() const
{
	std::ifstream inputStream(m_inputFileName);
	Reader reader(m_grammarType, inputStream);
	auto productionRules = reader.ReadProductionRules();
	auto result = productionRules.Determine();
	result.RenameStates();
	CSVWriter writer(m_outputFileName);
	writer.WriteMooreTable(result, "");
}

GrammarType ConsoleGrammarDetermination::GetGrammarTypeMode(const std::string& modeStr)
{
	std::string modeLowerStr = modeStr;
	std::transform(modeStr.begin(), modeStr.end(), modeLowerStr.begin(), [](unsigned char c) {
		return std::tolower(c);
	});

	if (modeLowerStr == "left")
	{
		return GrammarType::LeftSide;
	}
	else if (modeLowerStr == "right")
	{
		return GrammarType::RightSide;
	}
	else
	{
		throw std::invalid_argument("Wrong mode string was given\n");
	}
}
