#pragma once
#include "CSVReader.h"
#include "../Tables/MealyTable.h"

class MealyReader : public CSVReader
{
public:
	MealyReader(const std::string& inputFileName);

	MealyTable ReadMealyTable();
};