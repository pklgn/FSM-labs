#pragma once
#include <string>

std::string Trim(const std::string& str);

template <typename Iter>
std::string Join(Iter begin, Iter end, std::string const& separator = "")
{
	std::ostringstream result;
	if (begin != end)
		result << *begin++;
	while (begin != end)
		result << separator << *begin++;

	return result.str();
}
