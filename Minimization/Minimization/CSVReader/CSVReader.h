#pragma once
#include <string>
#include <fstream>
#include "../Tables/MealyTable.h"
#include "../Tables/MooreTable.h"

class CSVReader
{
public:
	CSVReader(const std::string& inputFileName);

	MealyTable ReadMealyTable();

	MooreTable ReadMooreTable();

protected:
	std::ifstream m_inputFile;
};
