#include "MooreReader.h"

MooreReader::MooreReader(const std::string& inputFileName)
	: CSVReader(inputFileName)
{
}

MooreTable MooreReader::ReadMooreTable()
{
	return MooreTable();
}
