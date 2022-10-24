#include <sstream>
#include "MealyReader.h"

MealyReader::MealyReader(const std::string& inputFileName)
	: CSVReader(inputFileName)
{
}

MealyTable MealyReader::ReadMealyTable()
{
	MealyTable mealyTable;
	std::string row;
	std::istringstream lineStream;
	char delimeter = ';';

	std::string rawStates;
	std::getline(m_inputFile, rawStates);
	lineStream.str(rawStates);

	State state;
	States states;
	TransitionTable transitionTable;
	while (std::getline(lineStream, state, delimeter))
	{
		if (!state.empty())
		{
			states.push_back(state);
			transitionTable[state];
		}
	}

	std::string tableField;
	InputSignals inputSignals;
	while (std::getline(m_inputFile, row))
	{
		lineStream.str(row);
		lineStream.clear();
		std::getline(lineStream, tableField, delimeter);
		inputSignals.push_back(tableField);

		for (size_t columnIndex = 0; std::getline(lineStream, tableField, delimeter); ++columnIndex)
		{
			transitionTable[states[columnIndex]].push_back(tableField);
		}
	}

	mealyTable.SetInputSignals(inputSignals);
	mealyTable.SetStates(states);
	mealyTable.SetTransitionTable(transitionTable);

	return mealyTable;
}
