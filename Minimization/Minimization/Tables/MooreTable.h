#pragma once
#include "MealyTable.h"

using OutputSignals = std::vector<Signal>;

class MooreTable : public MealyTable
{
public:
	OutputSignals GetOutputSignals() const;

	void SetOutputSignals(const OutputSignals&);

protected:
	OutputSignals m_outputSignals;
};