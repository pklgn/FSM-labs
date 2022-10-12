#include "../pch.h"

MooreReader::MooreReader(const std::string& inputCSVFileName): CSVReader(inputCSVFileName)
{
}

TableRow MooreReader::GetOutputSignals()
{
	return m_tableView.empty() ? TableRow() : m_tableView[0];
}

TableRow MooreReader::GetOrderedStates()
{
	return m_tableView.empty() ? TableRow() : m_tableView[1];
}

