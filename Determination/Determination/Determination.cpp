#include "pch.h"
#include "RegularGrammar/Reader/Reader.h"
#include "CSVWriter/CSVWriter.h"
#include "CSVReader/CSVReader.h"
#include "RegularGrammar/Console/Determination/ConsoleGrammarDetermination.h"
#include "Console/Determination/ConsoleDetermination.h"

using namespace RegularGrammar;

int main(int argc, char* argv[])
{
	try
	{
		if (argc == 4)
		{
			std::cout << "Assuming input params for regular grammar\n";
			ConsoleGrammarDetermination determination(argc, argv);
			determination.Run();
		}
		else if (argc == 3)
		{
			std::cout << "Assuming input params for state machine determination\n";
			ConsoleDetermination determination(argc, argv);
			determination.Run();
		}
		else
		{
			std::cout << "Unexpected amount of input params\n";
			return 1;
		}
	}
	catch (std::exception& ex)
	{
		std::cout << ex.what();

		return 1;
	}
}
