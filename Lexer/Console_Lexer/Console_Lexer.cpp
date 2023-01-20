// Console_Lexer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "../Lexer/Lexer/Lexer.h"

int main(int argc, char* argv[])
{
	try
	{
		if (argc == 2)
		{
			std::ifstream inputFile(argv[1]);
			if (!inputFile)
			{
				throw std::runtime_error("Unable to open file");
			}
			Lexer lexer;
			lexer.Run(inputFile);
			lexer.PrintTokens(std::cout);
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
