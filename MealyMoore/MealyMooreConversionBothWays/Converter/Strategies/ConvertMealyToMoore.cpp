#include "../../pch.h"
#include "ConvertMealyToMoore.h"

constexpr int HEADER_LINE_INDEX = 0;

std::string RetrieveDestinationState(const std::string& transitionTableField);
std::string RetrieveOutputSignal(const std::string& transitionTableField);

TableView ConvertMealyToMoore::RetrieveTransitionTable(TableView& srcTable)
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

TableView ConvertMealyToMoore::Convert(TableView& srcTable, std::string sign)
{
    auto transitionTable = RetrieveTransitionTable(srcTable);
    auto mealyStatesCount = transitionTable.front().size();
    auto inputSignalsCount = transitionTable.size();

    std::vector<std::string> uniqueStates;
    for (auto& row : transitionTable)
    {
        uniqueStates.insert(uniqueStates.end(), row.begin(), row.end());
    }

    std::sort(uniqueStates.begin(), uniqueStates.end());
    uniqueStates.erase(std::unique(uniqueStates.begin(), uniqueStates.end()), uniqueStates.end());

    TableView rawResult;

    rawResult.resize(inputSignalsCount);
    //for (auto& row : rawResult)
    for (size_t row = 0; row < inputSignalsCount; ++row)
    {
        rawResult[row].resize(uniqueStates.size() + 1);
        rawResult[row][0] = srcTable[row + 1][0];
    }

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

    TableRow outputSignals;
	outputSignals.resize(uniqueStates.size());
	for (size_t stateCounter = 0; stateCounter < uniqueStates.size(); ++stateCounter)
	{
		outputSignals[stateCounter] = RetrieveOutputSignal(uniqueStates[stateCounter]);
	}
	outputSignals.insert(outputSignals.begin(), "");

    for (size_t stateCounter = 0; stateCounter < uniqueStates.size(); ++stateCounter)
	{
		uniqueStates[stateCounter] = sign + std::to_string(stateCounter) + " (" + uniqueStates[stateCounter] + ")";
    }

    uniqueStates.insert(uniqueStates.begin(), "");

	rawResult.insert(rawResult.begin(), uniqueStates);
	rawResult.insert(rawResult.begin(), outputSignals);

    return rawResult;
}

std::string RetrieveDestinationState(const std::string& transitionTableField)
{
    std::string::size_type pos = transitionTableField.find('/');
    if (pos != std::string::npos)
    {
        return transitionTableField.substr(0, pos);
    }

    return transitionTableField;
}

std::string RetrieveOutputSignal(const std::string& transitionTableField)
{
	std::string::size_type pos = transitionTableField.find('/');
	if (pos != std::string::npos)
	{
		return transitionTableField.substr(pos + 1);
	}

    return transitionTableField;
}