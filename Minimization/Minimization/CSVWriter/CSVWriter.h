#pragma once
#include <string>
#include "../Tables/MealyTable.h"
#include "../Tables/MooreTable.h"

class CSVWriter
{
	CSVWriter(const std::string& outputFileName);

	void WriteMealyTable(const MealyTable&);

	void WriteMooreTable(const MooreTable&);
};