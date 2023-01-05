#include <sstream>
#include "CSVReader.h"
#include "../Common/StringCommon.h"

const char CSV_DELIMITER = ';';
const char MEALY_STATE_DELIMITER = '/';
const char DST_STATE_DELIMITER = ',';
const State EMPTY_STATE = "-";

template <typename T>
void InitTransitionTableWithStates(std::istringstream& iss, States& states, T& transitionTable);
std::string RetrieveOutputSignal(const std::string& tableField);
std::string RetrieveDestinationState(const std::string& tableField);

CSVReader::CSVReader(const std::string& inputFileName)
	: m_inputFile(inputFileName)
{
	if (!m_inputFile)
	{
		throw std::runtime_error("Unable to open specified file to read");
	}
}

MealyTable CSVReader::ReadMealyTable()
{
	std::string row;
	std::istringstream lineStream;

	std::string rawStates;
	std::getline(m_inputFile, rawStates);
	lineStream.str(rawStates);

	State state;
	States states;
	MealyTransitionTable transitionTable;
	InitTransitionTableWithStates(lineStream, states, transitionTable);

	std::string tableField;
	Signals inputSignals;
	while (std::getline(m_inputFile, row))
	{
		lineStream.str(row);
		lineStream.clear();
		std::getline(lineStream, tableField, CSV_DELIMITER);
		inputSignals.push_back(tableField);

		for (size_t columnIndex = 0; std::getline(lineStream, tableField, CSV_DELIMITER); ++columnIndex)
		{
			transitionTable[states[columnIndex]].commonStates.push_back({ RetrieveDestinationState(tableField) });
			transitionTable[states[columnIndex]].outputSignals.push_back(RetrieveOutputSignal(tableField));
		}
	}

	MealyTable mealyTable{ states, inputSignals, transitionTable };

	return mealyTable;
}

std::vector<State> ParseDstStates(const std::string& rawDstStates)
{
	size_t last = 0;
	size_t next = 0;
	State state;
	std::vector<State> dstStates;
	while ((next = rawDstStates.find(DST_STATE_DELIMITER, last)) != std::string::npos)
	{
		state = Trim(rawDstStates.substr(last, next - last));

		dstStates.push_back(state);
		last = next + 1;
	}
	dstStates.push_back(Trim(rawDstStates.substr(last)));

	return dstStates;
}

MooreTable CSVReader::ReadMooreTable()
{
	std::string rawOutputSignals;
	std::getline(m_inputFile, rawOutputSignals);
	std::istringstream lineStream(rawOutputSignals);

	std::string signal;
	Signals outputSignals;
	while (std::getline(lineStream, signal, CSV_DELIMITER))
	{
		outputSignals.push_back(signal);
	}
	if (signal.empty())
	{
		outputSignals.push_back(signal);
	}
	outputSignals.erase(outputSignals.begin());

	std::string row;
	std::getline(m_inputFile, row);
	lineStream.str(row);
	lineStream.clear();
	State state;
	States states;
	MooreTransitionTable transitionTable;
	InitTransitionTableWithStates(lineStream, states, transitionTable);

	std::string tableField;
	Signals inputSignals;
	while (std::getline(m_inputFile, row))
	{
		lineStream.str(row);
		lineStream.clear();
		std::getline(lineStream, tableField, CSV_DELIMITER);
		inputSignals.push_back(tableField);

		for (size_t columnIndex = 0; std::getline(lineStream, tableField, CSV_DELIMITER); ++columnIndex)
		{
			if (tableField == EMPTY_STATE)
			{
				transitionTable[states[columnIndex]].commonStates.push_back({});
			}
			else
			{
				transitionTable[states[columnIndex]].commonStates.push_back(ParseDstStates(tableField));
			}
		}
	}

	MooreTable mooreTable{ states, inputSignals, transitionTable, outputSignals };

	return mooreTable;
}

std::string RetrieveDestinationState(const std::string& tableField)
{
	std::string::size_type pos = tableField.find(MEALY_STATE_DELIMITER);
	if (pos != std::string::npos)
	{
		return tableField.substr(0, pos);
	}

	return tableField;
}

std::string RetrieveOutputSignal(const std::string& tableField)
{
	std::string::size_type pos = tableField.find(MEALY_STATE_DELIMITER);
	if (pos != std::string::npos)
	{
		return tableField.substr(pos + 1);
	}

	return tableField;
}

template<typename T>
void InitTransitionTableWithStates(std::istringstream& iss, States& states, T& transitionTable)
{
	std::string state;
	while (std::getline(iss, state, CSV_DELIMITER))
	{
		if (!state.empty())
		{
			states.push_back(state);
			transitionTable[state];
		}
	}
}
