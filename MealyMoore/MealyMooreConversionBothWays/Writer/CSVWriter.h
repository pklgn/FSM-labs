#pragma once

#include <string>
#include "../Reader/ICSVReader.h"

class CSVWriter
{
public:
	void WriteCSVTable(const TableView&, const std::string& outputFilename);
};