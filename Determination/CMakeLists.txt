cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

project(Determination LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME}
    "Determination/Common/StringCommon.h"
    "Determination/Console/Determination/ConsoleDetermination.h"
    "Determination/CSVReader/CSVReader.h"
    "Determination/CSVWriter/CSVWriter.h"
    "Determination/pch.h"
    "Determination/RegularGrammar/Common/GrammarTypes.h"
    "Determination/RegularGrammar/Console/Determination/ConsoleGrammarDetermination.h"
    "Determination/RegularGrammar/ProductionRules/ProductionRule/ProductionRule.h"
    "Determination/RegularGrammar/ProductionRules/ProductionRule/RightPart/RightPart.h"
    "Determination/RegularGrammar/ProductionRules/ProductionRules.h"
    "Determination/RegularGrammar/Reader/Reader.h"
    "Determination/Tables/FSMTable.hpp"
    "Determination/Tables/MealyTable.h"
    "Determination/Tables/MooreTable.h"
    "Determination/Common/StringCommon.cpp"
    "Determination/Console/Determination/ConsoleDetermination.cpp"
    "Determination/CSVReader/CSVReader.cpp"
    "Determination/CSVWriter/CSVWriter.cpp"
    "Determination/Determination.cpp"
    "Determination/pch.cpp"
    "Determination/RegularGrammar/Console/Determination/ConsoleGrammarDetermination.cpp"
    "Determination/RegularGrammar/ProductionRules/ProductionRule/RightPart/RightPart.cpp"
    "Determination/RegularGrammar/ProductionRules/ProductionRules.cpp"
    "Determination/RegularGrammar/Reader/Reader.cpp"
    "Determination/Tables/MealyTable.cpp"
    "Determination/Tables/MooreTable.cpp"
)