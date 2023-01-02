#include "RightPart.h"

RegularGrammar::RightPart::RightPart(GrammarType type, const RawRightPart& rawRightPart)
{
	switch (type)
	{
	case GrammarType::LeftSide:
		if (rawRightPart.size() == 2)
		{
			nonTerminal = rawRightPart.front();
			terminal = rawRightPart.back();
		}
		else if (rawRightPart.size() == 1)
		{
			terminal = rawRightPart.front();
		}
		break;
	case GrammarType::RightSide:
		if (rawRightPart.size() == 2)
		{
			terminal = rawRightPart.front();
			nonTerminal = rawRightPart.back();
		}
		else if (rawRightPart.size() == 1)
		{
			terminal = rawRightPart.front();
		}
		break;
	}
}
