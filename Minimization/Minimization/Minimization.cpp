#include "CSVReader/MealyReader.h"

int main()
{
	CSVReader reader("INPUT.TXT");

	auto table = reader.ReadMooreTable();
	table.Minimize();

}