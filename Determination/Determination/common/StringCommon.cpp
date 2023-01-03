#include "../pch.h"
#include "StringCommon.h"

std::string Trim(const std::string& str)
{
	auto result = str;
	result.erase(str.find_last_not_of(' ') + 1); // suffixing spaces
	result.erase(0, str.find_first_not_of(' ')); // prefixing spaces

	return result;
}

std::string Join(const std::vector<std::string> values)
{
	std::stringstream buffer;
	std::copy(values.begin(), values.end(), std::ostream_iterator<std::string>(buffer));

	return buffer.str();
}