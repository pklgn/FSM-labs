#include <iostream>
#include <algorithm>
#include "CSVReader/CSVReader.h"
#include "CSVWriter/CSVWriter.h"

enum class MinimizationMode
{
	MEALY,
	MOORE,
	NO_MODE,
};

MinimizationMode GetMinimizationMode(const std::string& modeStr);

int main(int argc, char* argv[])
{
	if (argc < 4)
	{
		std::cout << "Not enough params\n";

		return 1;
	}
	else if (argc > 4)
	{
		std::cout << "Extra parameters will be ignored\n";
	}

	MinimizationMode mode = GetMinimizationMode(argv[1]);

	if (mode == MinimizationMode::NO_MODE)
	{
		std::cout << "Wrong mode value was given\n";

		return 1;
	}
	
	try
	{
		CSVReader reader(argv[2]);
		CSVWriter writer(argv[3]);
		switch (mode)
		{
		case MinimizationMode::MEALY:
		{
			auto mealyTable = reader.ReadMealyTable();
			mealyTable.Minimize();
			writer.WriteMealyTable(mealyTable);

			return 0;
		}
		case MinimizationMode::MOORE:
		{
			auto mooreTable = reader.ReadMooreTable();
			mooreTable.Minimize();
			writer.WriteMooreTable(mooreTable);

			return 0;
		}
		}
	}
	catch (std::exception& ex)
	{
		std::cout << ex.what();

		return 1;
	}
}

MinimizationMode GetMinimizationMode(const std::string& modeStr)
{
	std::string modeLowerStr = modeStr;
	std::transform(modeStr.begin(), modeStr.end(), modeLowerStr.begin(), [](unsigned char c) {
		return std::tolower(c);
	});

	if (modeLowerStr == "mealy")
	{
		return MinimizationMode::MEALY;
	}
	else if (modeLowerStr == "moore")
	{
		return MinimizationMode::MOORE;
	}

	return MinimizationMode::NO_MODE;
}