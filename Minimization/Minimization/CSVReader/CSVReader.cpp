#include <sstream>
#include "CSVReader.h"

const char CSV_DELIMETER = ';';

CSVReader::CSVReader(const std::string& inputFileName)
	: m_inputFile(inputFileName)
{
}

MealyTable CSVReader::ReadMealyTable()
{
	MealyTable mealyTable;
	std::string row;
	std::istringstream lineStream;

	std::string rawStates;
	std::getline(m_inputFile, rawStates);
	lineStream.str(rawStates);

	State state;
	States states;
	TransitionTable transitionTable;
	while (std::getline(lineStream, state, CSV_DELIMETER))
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
		std::getline(lineStream, tableField, CSV_DELIMETER);
		inputSignals.push_back(tableField);

		for (size_t columnIndex = 0; std::getline(lineStream, tableField, CSV_DELIMETER); ++columnIndex)
		{
			transitionTable[states[columnIndex]].push_back(tableField);
		}
	}

	mealyTable.SetInputSignals(inputSignals);
	mealyTable.SetStates(states);
	mealyTable.SetTransitionTable(transitionTable);

	return mealyTable;
}

MooreTable CSVReader::ReadMooreTable()
{
	std::string rawOutputSignals;
	std::getline(m_inputFile, rawOutputSignals);
	std::istringstream lineStream(rawOutputSignals);

	std::string signal;
	OutputSignals outputSignals;
	while (std::getline(lineStream, signal, CSV_DELIMETER))
	{
		if (!signal.empty())
		{
			outputSignals.push_back(signal);
		}
	}

	MealyTable mealyTable = ReadMealyTable();

	MooreTable mooreTable;
	mooreTable.SetInputSignals(mealyTable.GetInputSignals());
	mooreTable.SetOutputSignals(outputSignals);
	mooreTable.SetStates(mealyTable.GetStates());
	mooreTable.SetTransitionTable(mealyTable.GetTransitionTable());

	return mooreTable;
}
