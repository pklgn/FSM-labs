#pragma once

#include "../../Reader/ICSVReader.h"

class IConvertStrategy
{
public:
	virtual TableView Convert(const TableView&, std::string sign = "S") = 0;

	virtual ~IConvertStrategy() = default;

protected:
	virtual TableView RetrieveTransitionTable(const TableView&) = 0;
};