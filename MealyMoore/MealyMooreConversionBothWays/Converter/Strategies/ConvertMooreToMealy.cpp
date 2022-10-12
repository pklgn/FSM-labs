#include "../../pch.h"
#include "ConvertMooreToMealy.h"

TableView ConvertMooreToMealy::RetrieveTransitionTable(TableView& srcTable)
{
	TableView result = srcTable;

	// remove headers
	result.erase(result.begin(), result.begin() + 2);

	// remove input signals column
	for (auto& row : result)
	{
		row.erase(row.begin());
	}

	return result;
}

TableView ConvertMooreToMealy::Convert(TableView& srcTable, std::string sign)
{
	TableView resultMealyTable = RetrieveTransitionTable(srcTable);

	for (auto& row : resultMealyTable)
	{
		for (auto& field : row)
		{
			auto it = std::find(srcTable[1].begin(), srcTable[1].end(), field);

			auto outputSignalColumn = it - srcTable[1].begin();

			field = field + "/" + srcTable[0][outputSignalColumn];
		}
	}

	for (size_t rowCounter = 0; rowCounter < resultMealyTable.size(); ++rowCounter)
	{
		resultMealyTable[rowCounter].insert(resultMealyTable[rowCounter].begin(), srcTable[rowCounter + 2][0]);
	}

	resultMealyTable.insert(resultMealyTable.begin(), srcTable[1]);

    return resultMealyTable;
}
