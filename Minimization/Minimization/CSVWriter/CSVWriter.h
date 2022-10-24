#pragma once
#include <string>
#include <fstream>
#include "../Tables/FSMTable.hpp"
#include "../Tables/MealyTable.h"
#include "../Tables/MooreTable.h"

class CSVWriter
{
public:
	CSVWriter(const std::string& outputFileName);

	void WriteMealyTable(const MealyTable&);

	void WriteMooreTable(const MooreTable&);

protected:
	void WriteStatesHeader(const States&);
	void WriteMooreOutputSignalsHeader(const Signals&);

	std::ofstream m_outputStream;
};