#include <sstream>
#include "CSVReader.h"

const char CSV_DELIMITER = ';';
const char MEALY_STATE_DELIMITER = '/';

template <typename T>
void InitTransitionTableWithStates(std::istringstream& iss, States& states, T& transitionTable);
std::string RetrieveOutputSignal(const std::string& tableField);
std::string RetrieveDestinationState(const std::string& tableField);

CSVReader::CSVReader(const std::string& inputFileName)
	: m_inputFile(inputFileName)
{
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
			transitionTable[states[columnIndex]].commonStates.push_back(RetrieveDestinationState(tableField));
			transitionTable[states[columnIndex]].outputSignals.push_back(RetrieveOutputSignal(tableField));
		}
	}

	MealyTable mealyTable{ states, inputSignals, transitionTable };

	return mealyTable;
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
		if (!signal.empty())
		{
			outputSignals.push_back(signal);
		}
	}

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
		// считываем построчно: сначала заносим информацию о входном сигнале
		// так как он находится в начале каждой строки таблицы
		// затем заполняем ячейки таблиц состояниями, в которые возможен переход по указанному символу
		lineStream.str(row);
		lineStream.clear();
		std::getline(lineStream, tableField, CSV_DELIMITER);
		inputSignals.push_back(tableField);

		for (size_t columnIndex = 0; std::getline(lineStream, tableField, CSV_DELIMITER); ++columnIndex)
		{
			transitionTable[states[columnIndex]].commonStates.push_back(tableField);
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
