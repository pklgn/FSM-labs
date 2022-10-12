#include "../pch.h"
#include "FSMConverter.h"
#include "Strategies/ConvertMealyToMoore.h"
#include "Strategies/ConvertMooreToMealy.h"

FSMConverter::FSMConverter(Mode mode)
{
    switch (mode)
    {
    case Mode::MEALY_TO_MOORE:
        m_convertStrategy = std::make_unique<ConvertMealyToMoore>();
        break;
    case Mode::MOORE_TO_MEALY:
        m_convertStrategy = std::make_unique<ConvertMooreToMealy>();
        break;
    }
}

TableView FSMConverter::Convert(TableView& srcTable)
{
    return m_convertStrategy->Convert(srcTable);
}

void FSMConverter::SetConvertStrategy(ConvertStrategyPtr convertStrategy)
{
    m_convertStrategy = std::move(convertStrategy);
}
