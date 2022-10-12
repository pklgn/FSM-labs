#pragma once

class MooreReader : public CSVReader
{
public:
	MooreReader(const std::string& inputCSVFileName);

	TableRow GetOutputSignals();

	TableRow GetOrderedStates();
};