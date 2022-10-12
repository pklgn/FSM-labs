#include "../pch.h"
#include "CSVWriter.h"

void CSVWriter::WriteCSVTable(const TableView& table, const std::string& outputFilename)
{
	std::ofstream os(outputFilename);
	csv::DelimWriter<std::ofstream, ';', '"', true> writer(os);

	for (auto& row : table)
	{
		writer << row;
	}
}
