#include "pch.h"
#include "RegularGrammar/Reader/RegularGrammarReader.h"

using namespace RegularGrammar;

int main(int argc, char* argv[])
{
	std::ifstream input("INPUT.TXT");
	Reader reader(input);
	auto rules = reader.ReadProductionRules();
}

