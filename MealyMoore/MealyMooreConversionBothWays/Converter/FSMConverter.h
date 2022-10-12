#pragma once
#include <memory>
#include "../Reader/ICSVReader.h"
#include "Strategies/IConvertStrategy.h"

typedef std::unique_ptr<IConvertStrategy> ConvertStrategyPtr;

class FSMConverter
{
public:
	enum class Mode
	{
		MEALY_TO_MOORE,
		MOORE_TO_MEALY,
	};

	FSMConverter(Mode);

	TableView Convert(TableView& srcTable);

	void SetConvertStrategy(ConvertStrategyPtr);

protected:
	ConvertStrategyPtr m_convertStrategy;
};
