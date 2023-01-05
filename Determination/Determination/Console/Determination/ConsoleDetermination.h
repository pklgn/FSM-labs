#pragma once
#include <string>

class ConsoleDetermination
{
public:
	ConsoleDetermination(int argc, char* argv[]);

	void Run() const;

private:
	const std::string& m_inputFileName;
	const std::string& m_outputFileName;
};
