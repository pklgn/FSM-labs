#pragma once
#include <string>
#include <fstream>
#include "../Tables/FSMTable.hpp"
#include "../Tables/MealyTable.h"
#include "../Tables/MooreTable.h"

class CSVWriter
{
public:
	static const std::string STATE_PREFIX;
	CSVWriter(const std::string& outputFileName);

	void WriteMealyTable(const MealyTable&, const std::string& prefix = STATE_PREFIX);

	void WriteMooreTable(const MooreTable&, const std::string& prefix = STATE_PREFIX);

protected:
	void WriteStatesHeader(const States&, const std::string& prefix = STATE_PREFIX);
	void WriteMooreOutputSignalsHeader(const Signals&);

	std::ofstream m_outputStream;
};