#include "CSVReader/MealyReader.h"

int main()
{
	CSVReader reader("INPUT.TXT");

	auto table = reader.ReadMealyTable();
	table.Minimize();

}