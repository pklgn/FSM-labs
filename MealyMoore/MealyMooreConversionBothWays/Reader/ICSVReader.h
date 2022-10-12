#pragma once
#include <vector>

typedef std::vector<std::string> TableRow;

typedef std::vector<TableRow> TableView;

class ICSVReader
{
public:
	virtual TableView ReadCSVTable() = 0;

	virtual TableView GetTableView() const = 0;
};

