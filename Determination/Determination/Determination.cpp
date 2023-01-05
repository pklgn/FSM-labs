#include "pch.h"
#include "RegularGrammar/Reader/Reader.h"
#include "CSVWriter/CSVWriter.h"
#include "CSVReader/CSVReader.h"

using namespace RegularGrammar;

int main(int argc, char* argv[])
{
	CSVReader reader("INPUT.TXT");
	auto moore = reader.ReadMooreTable();
	auto result = moore.Determine();
	result.RenameStates();

	CSVWriter writer("OUTPUT.TXT");
	writer.WriteMooreTable(result, "");
}
