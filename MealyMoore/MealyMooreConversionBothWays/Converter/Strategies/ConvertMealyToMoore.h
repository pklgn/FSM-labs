#pragma once

#include "IConvertStrategy.h"

class ConvertMealyToMoore : public IConvertStrategy
{
public:
	TableView Convert(TableView&, std::string sign) override;

protected:
	TableView RetrieveTransitionTable(TableView&) override;
};