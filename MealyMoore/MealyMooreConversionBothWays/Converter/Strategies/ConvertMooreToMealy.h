#pragma once

#include "IConvertStrategy.h"

class ConvertMooreToMealy : public IConvertStrategy
{
public:
	TableView Convert(TableView&, std::string sign) override;

protected:
	TableView RetrieveTransitionTable(TableView&) override;
};