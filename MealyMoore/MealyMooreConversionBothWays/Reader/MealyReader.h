#pragma once

class MealyReader : public CSVReader
{
public:
	MealyReader(const std::string& inputCSVFileName);

	TableRow GetOrderedStates();
};