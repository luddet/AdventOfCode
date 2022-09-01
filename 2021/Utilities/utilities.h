#pragma once
#include <istream>
#include <vector>
#include <string>
#include <limits>
#include <sstream>

#include "dllexport.h"


template<typename T>
auto readItems(std::istream& is, const char delimiter = ' ')
{
	std::vector<T> result;
	while (!is.eof())
	{
		T i;
		is >> i;
		if (is.bad())
			break;
		
		if (is.fail())
		{
			is.clear();
			is.ignore( (std::numeric_limits<std::streamsize>::max)(), delimiter);
			continue;
		}

		result.push_back(i);
	}
	return result;
}

template<typename T>
auto readItems(std::string& str, const char delimiter = ' ')
{
	std::istringstream ss(str);
	return readItems<T>(ss, delimiter);
}


DLLEXPORT std::vector<int> readInts(std::istream& os);
DLLEXPORT std::vector<std::string> readLines(const std::string& filePath);
