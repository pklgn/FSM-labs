#pragma once

#include "IConvertStrategy.h"

class ConvertMooreToMealy : public IConvertStrategy
{
public:
	TableView Convert(const TableView&, std::string sign) override;

protected:
	TableView RetrieveTransitionTable(const TableView&) override;
};