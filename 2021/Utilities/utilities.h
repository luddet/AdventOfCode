#pragma once
#include <vector>
#include <string>
#include <istream>

#include "dllexport.h"

template<typename T>
auto readItems(std::istream& os)
{
	std::vector<T> result;
	while (!os.eof())
	{
		T i;
		os >> i;
		result.push_back(i);
	}
	return result;
}

DLLEXPORT std::vector<int> readInts(std::istream& os);
DLLEXPORT std::vector<std::string> readLines(const std::string& filePath);
