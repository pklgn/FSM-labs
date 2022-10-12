// Console_MealyMooreConversionBothWays.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "../MealyMooreConversionBothWays/pch.h"
#include "../MealyMooreConversionBothWays/Reader/CSVReader.h"
#include "../MealyMooreConversionBothWays/Writer/CSVWriter.h"
#include "../MealyMooreConversionBothWays/Converter/FSMConverter.h"
#include "../MealyMooreConversionBothWays/Converter/Strategies/ConvertMealyToMoore.h"

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

	FSMConverter::Mode mode;
	if (argv[1] == FSMConverter::ModeToString(FSMConverter::Mode::MOORE_TO_MEALY))
	{
		mode = FSMConverter::Mode::MOORE_TO_MEALY;
	}
	else if (argv[1] == FSMConverter::ModeToString(FSMConverter::Mode::MEALY_TO_MOORE))
	{
		mode = FSMConverter::Mode::MEALY_TO_MOORE;
	}
	else
	{
		std::cout << "Wrong mode value\n";

		return 1;
	}

    CSVReader reader(argv[2]);

	try
	{
		TableView tableView = reader.ReadCSVTable();
		FSMConverter converter(mode);
		TableView result = converter.Convert(tableView);

		CSVWriter writer;
		writer.WriteCSVTable(result, argv[3]);
	}
	catch (std::exception& ex)
	{
		std::cout << ex.what();

		return 1;
	}
}
