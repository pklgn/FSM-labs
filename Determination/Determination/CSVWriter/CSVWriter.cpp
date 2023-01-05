#include <numeric>
#include "CSVWriter.h"

const char CSV_DELIMITER = ';';
const char MEALY_STATE_DELIMITER = '/';
const char NO_STATE_STR = '-';

const std::string CSVWriter::STATE_PREFIX = "q";

CSVWriter::CSVWriter(const std::string& outputFileName)
	: m_outputStream(outputFileName)
{
}

void CSVWriter::WriteMealyTable(const MealyTable& mealyTable, const std::string& prefix)
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
			auto currStates = transitionTable[state].commonStates[line];
			auto currStatesStr = std::accumulate(currStates.begin(), currStates.end(), std::string(""));
			if (currStatesStr.empty())
			{
				currStatesStr = NO_STATE_STR;
			}
			m_outputStream << CSV_DELIMITER
						   << prefix << currStatesStr
						   << MEALY_STATE_DELIMITER
						   << transitionTable[state].outputSignals[line];
		}
		m_outputStream << std::endl;
	}
}

void CSVWriter::WriteMooreTable(const MooreTable& mooreTable, const std::string& prefix)
{
	WriteMooreOutputSignalsHeader(mooreTable.GetOutputSignals());
	auto states = mooreTable.GetStates();
	std::sort(states.begin(), states.end());
	WriteStatesHeader(states, prefix);

	auto inputSignals = mooreTable.GetInputSignals();
	auto lineCount = inputSignals.size();
	auto transitionTable = mooreTable.GetTransitionTable();
	for (auto line = 0; line < lineCount; ++line)
	{
		m_outputStream << inputSignals[line];
		for (auto& state: states)
		{
			auto currStates = transitionTable[state].commonStates[line];
			auto currStatesStr = std::accumulate(currStates.begin(), currStates.end(), std::string(""));
			if (currStatesStr.empty())
			{
				currStatesStr = NO_STATE_STR;
			}
			m_outputStream << CSV_DELIMITER
						   << prefix << currStatesStr;
		}
		m_outputStream << std::endl;
	}
}

void CSVWriter::WriteStatesHeader(const States& states, const std::string& prefix)
{
	for (auto& state : states)
	{
		m_outputStream << CSV_DELIMITER << prefix + state;
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
