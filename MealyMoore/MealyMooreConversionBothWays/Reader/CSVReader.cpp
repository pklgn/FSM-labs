#include "../pch.h"
#include "CSVReader.h"

CSVReader::CSVReader(const std::string& inputCSVFileName): m_inputCSVFileName(inputCSVFileName)
{
}

TableView CSVReader::ReadCSVTable()
{
    csv::CSVFormat format;
    format.no_header()
        .delimiter(';')
        .trim({' '});

    csv::CSVReader reader(m_inputCSVFileName, format);

    for (csv::CSVRow& row : reader) {
        TableRow parsedRow;
        for (csv::CSVField& field : row) {
            parsedRow.push_back(field.get<>());
        }
        m_tableView.push_back(parsedRow);
    }

	return m_tableView;
}
