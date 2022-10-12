#include "../../pch.h"
#include "ConvertMealyToMoore.h"

constexpr int HEADER_LINE_INDEX = 0;
constexpr int INPUT_SIGNAL_COLUMN_INDEX = 0;

std::string RetrieveDestinationState(const std::string& tableField)
{
	std::string::size_type pos = tableField.find('/');
	if (pos != std::string::npos)
	{
		return tableField.substr(0, pos);
	}

	return tableField;
}

std::string RetrieveOutputSignal(const std::string& tableField)
{
	std::string::size_type pos = tableField.find('/');
	if (pos != std::string::npos)
	{
		return tableField.substr(pos + 1);
	}

	return tableField;
}

TableView ConvertMealyToMoore::RetrieveTransitionTable(const TableView& srcTable)
{
    TableView result = srcTable;

    //remove header
    result.erase(result.begin());

    //remove input signals column
    for (auto& row : result)
    {
        row.erase(row.begin());
    }

    return result;
}

TableView ConvertMealyToMoore::Convert(const TableView& srcTable, std::string sign)
{
    auto transitionTable = RetrieveTransitionTable(srcTable);
    auto mealyStatesCount = transitionTable.front().size();
    auto inputSignalsCount = transitionTable.size();

    //remove duplicates from transition table
    std::vector<std::string> uniqueStates;
    for (auto& row : transitionTable)
    {
        uniqueStates.insert(uniqueStates.end(), row.begin(), row.end());
    }
    std::sort(uniqueStates.begin(), uniqueStates.end());
    uniqueStates.erase(std::unique(uniqueStates.begin(), uniqueStates.end()), uniqueStates.end());

    //shape result table with unique columns count for transitions
    TableView rawResult;
    rawResult.resize(inputSignalsCount);
    for (size_t row = 0; row < inputSignalsCount; ++row)
    {
        rawResult[row].resize(uniqueStates.size() + 1);
		rawResult[row][INPUT_SIGNAL_COLUMN_INDEX] = srcTable[row + 1][INPUT_SIGNAL_COLUMN_INDEX];
    }

    //fill transition table with renamed states
    for (size_t stateCount = 1; stateCount < uniqueStates.size() + 1; ++stateCount)
    {
        auto destinationState = RetrieveDestinationState(uniqueStates[stateCount - 1]);

        auto columnIt = std::find(srcTable[HEADER_LINE_INDEX].begin(), srcTable[HEADER_LINE_INDEX].end(), destinationState);

        auto columnIndex = columnIt - srcTable[HEADER_LINE_INDEX].begin() - 1;

        std::vector<std::string> mealyStatesByColumn;
        for (auto& row : transitionTable)
        {
            mealyStatesByColumn.push_back(row[columnIndex]);
        }

        for (size_t inputSignalIndex = 0; inputSignalIndex < inputSignalsCount; ++inputSignalIndex)
        {
            auto it = std::find(uniqueStates.begin(), uniqueStates.end(), mealyStatesByColumn[inputSignalIndex]);

            rawResult[inputSignalIndex][stateCount] = sign + std::to_string(std::distance(uniqueStates.begin(), it));
        }
    }

    //create output signals row
    TableRow outputSignals;
	outputSignals.resize(uniqueStates.size());
	for (size_t stateCounter = 0; stateCounter < uniqueStates.size(); ++stateCounter)
	{
		outputSignals[stateCounter] = RetrieveOutputSignal(uniqueStates[stateCounter]);
	}
	outputSignals.insert(outputSignals.begin(), "");

    //rename states
    for (size_t stateCounter = 0; stateCounter < uniqueStates.size(); ++stateCounter)
	{
		uniqueStates[stateCounter] = sign + std::to_string(stateCounter) + " (" + uniqueStates[stateCounter] + ")";
    }
    uniqueStates.insert(uniqueStates.begin(), "");

	rawResult.insert(rawResult.begin(), uniqueStates);
	rawResult.insert(rawResult.begin(), outputSignals);

    return rawResult;
}
