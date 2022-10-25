#include "CSVWriter.h"

const char CSV_DELIMETER = ';';
const char MEALY_STATE_DELIMETER = '/';

const char STATE_PREFIX = 'q';

CSVWriter::CSVWriter(const std::string& outputFileName)
	: m_outputStream(outputFileName)
{
}

void CSVWriter::WriteMealyTable(const MealyTable& mealyTable)
{
	WriteStatesHeader(mealyTable.GetStates());

	auto inputSignals = mealyTable.GetInputSignals();
	auto lineCount = inputSignals.size();
	auto transitionTable = mealyTable.GetTransitionTable();
	for (auto line = 0; line < lineCount; ++line)
	{
		m_outputStream << inputSignals[line];
		for (auto& [state, transitions] : transitionTable)
		{
			m_outputStream << CSV_DELIMETER
						   << STATE_PREFIX << transitions.commonStates[line]
						   << MEALY_STATE_DELIMETER
						   << transitions.outputSignals[line];
		}
		m_outputStream << std::endl;
	}
}

void CSVWriter::WriteMooreTable(const MooreTable& mooreTable)
{
	WriteMooreOutputSignalsHeader(mooreTable.GetOutputSignals());
	WriteStatesHeader(mooreTable.GetStates());

	auto inputSignals = mooreTable.GetInputSignals();
	auto lineCount = inputSignals.size();
	auto transitionTable = mooreTable.GetTransitionTable();
	for (auto line = 0; line < lineCount; ++line)
	{
		m_outputStream << inputSignals[line];
		for (auto& [state, transitions] : transitionTable)
		{
			m_outputStream << CSV_DELIMETER
						   << STATE_PREFIX << transitions.commonStates[line];
		}
		m_outputStream << std::endl;
	}
}

void CSVWriter::WriteStatesHeader(const States& states)
{
	for (auto& state : states)
	{
		m_outputStream << CSV_DELIMETER << STATE_PREFIX + state;
	}
	m_outputStream << std::endl;
}

void CSVWriter::WriteMooreOutputSignalsHeader(const Signals& outputSignals)
{
	for (auto& signal : outputSignals)
	{
		m_outputStream << CSV_DELIMETER << signal;
	}
	m_outputStream << std::endl;
}
