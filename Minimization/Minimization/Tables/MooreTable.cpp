#include "MooreTable.h"

OutputSignals MooreTable::GetOutputSignals() const
{
	return m_outputSignals;
}

void MooreTable::SetOutputSignals(const OutputSignals& outputSignals)
{
	m_outputSignals = outputSignals;
}
