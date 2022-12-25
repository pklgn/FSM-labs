#include "CSVWriter.h"

const char CSV_DELIMITER = ';';
const char MEALY_STATE_DELIMITER = '/';

const char STATE_PREFIX = 'q';

CSVWriter::CSVWriter(const std::string& outputFileName)
	: m_outputStream(outputFileName)
{
}

void CSVWriter::WriteMealyTable(const MealyTable& mealyTable)
{
	auto states = mealyTable.GetStates();
	std::sort(states.begin(), states.end());
	WriteStatesHeader(states);

	auto inputSignals = mealyTable.GetInputSignals();
	auto lineCount = inputSignals.size();
	auto transitionTable = mealyTable.GetTransitionTable();
	for (auto line = 0; line < lineCount; ++line)
	{
		m_outputStream << inputSignals[line];
		for (auto& state: states)
		{
			m_outputStream << CSV_DELIMITER
						   << STATE_PREFIX << transitionTable[state].commonStates[line]
						   << MEALY_STATE_DELIMITER
						   << transitionTable[state].outputSignals[line];
		}
		m_outputStream << std::endl;
	}
}

void CSVWriter::WriteMooreTable(const MooreTable& mooreTable)
{
	WriteMooreOutputSignalsHeader(mooreTable.GetOutputSignals());
	auto states = mooreTable.GetStates();
	std::sort(states.begin(), states.end());
	WriteStatesHeader(states);

	auto inputSignals = mooreTable.GetInputSignals();
	auto lineCount = inputSignals.size();
	auto transitionTable = mooreTable.GetTransitionTable();
	for (auto line = 0; line < lineCount; ++line)
	{
		m_outputStream << inputSignals[line];
		for (auto& state: states)
		{
			m_outputStream << CSV_DELIMITER
						   << STATE_PREFIX << transitionTable[state].commonStates[line];
		}
		m_outputStream << std::endl;
	}
}

void CSVWriter::WriteStatesHeader(const States& states)
{
	for (auto& state : states)
	{
		m_outputStream << CSV_DELIMITER << STATE_PREFIX + state;
	}
	m_outputStream << std::endl;
}

void CSVWriter::WriteMooreOutputSignalsHeader(const Signals& outputSignals)
{
	for (auto& signal : outputSignals)
	{
		m_outputStream << CSV_DELIMITER << signal;
	}
	m_outputStream << std::endl;
}
