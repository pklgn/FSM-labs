#include "ConsoleDetermination.h"
#include "../../CSVReader/CSVReader.h"
#include "../../CSVWriter/CSVWriter.h"

ConsoleDetermination::ConsoleDetermination(int argc, char* argv[])
	: m_inputFileName(argv[1])
	, m_outputFileName(argv[2])
{
}

void ConsoleDetermination::Run() const
{
	CSVReader reader(m_inputFileName);
	auto moore = reader.ReadMooreTable();
	auto result = moore.Determine();
	result.RenameStates();
	CSVWriter writer(m_outputFileName);
	writer.WriteMooreTable(result, "");
}
