#include "../pch.h"

MealyReader::MealyReader(const std::string& inputCSVFileName): CSVReader(inputCSVFileName)
{
}

TableRow MealyReader::GetOrderedStates()
{
    return m_tableView.empty() ? TableRow() : m_tableView[0];
}
