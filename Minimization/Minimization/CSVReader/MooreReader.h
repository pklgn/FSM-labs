#pragma once
#include "CSVReader.h"
#include "../Tables/MooreTable.h"

class MooreReader : public CSVReader
{
public:
	MooreReader(const std::string& inputFileName);

	MooreTable ReadMooreTable();
};