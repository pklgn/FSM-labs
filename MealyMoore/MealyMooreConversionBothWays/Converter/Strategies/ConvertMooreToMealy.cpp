#include "../../pch.h"
#include "ConvertMooreToMealy.h"

constexpr int HEADERS_COUNT = 2;
constexpr int STATE_HEADER_LINE = 1;
constexpr int OUTPUT_SIGNALS_HEADER_LINE = 0;
constexpr int INPUT_SIGNALS_COLUMN_INDEX = 0;

const std::string MEALY_STATE_DELIMETER = "/";

TableView ConvertMooreToMealy::RetrieveTransitionTable(const TableView& srcTable)
{
	TableView result = srcTable;

	// remove headers
	result.erase(result.begin(), result.begin() + HEADERS_COUNT);

	// remove input signals column
	for (auto& row : result)
	{
		row.erase(row.begin());
	}

	return result;
}

TableView ConvertMooreToMealy::Convert(const TableView& srcTable, std::string sign)
{
	TableView resultMealyTable = RetrieveTransitionTable(srcTable);

	for (auto& row : resultMealyTable)
	{
		for (auto& field : row)
		{
			auto it = std::find(srcTable[STATE_HEADER_LINE].begin(), srcTable[STATE_HEADER_LINE].end(), field);

			auto outputSignalColumn = std::distance(srcTable[STATE_HEADER_LINE].begin(), it);

			field = field + MEALY_STATE_DELIMETER + srcTable[OUTPUT_SIGNALS_HEADER_LINE][outputSignalColumn];
		}
	}

	for (size_t rowCounter = 0; rowCounter < resultMealyTable.size(); ++rowCounter)
	{
		resultMealyTable[rowCounter].insert(resultMealyTable[rowCounter].begin(), srcTable[rowCounter + HEADERS_COUNT][INPUT_SIGNALS_COLUMN_INDEX]);
	}

	resultMealyTable.insert(resultMealyTable.begin(), srcTable[STATE_HEADER_LINE]);

    return resultMealyTable;
}
