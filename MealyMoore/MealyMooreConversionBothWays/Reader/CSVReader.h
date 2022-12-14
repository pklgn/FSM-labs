#pragma once

#include "ICSVReader.h"

class CSVReader : public ICSVReader
{
public:
	CSVReader(const std::string& inputCSVFileName);

	TableView ReadCSVTable() override;

protected:
	std::string m_inputCSVFileName;
	TableView m_tableView;
};