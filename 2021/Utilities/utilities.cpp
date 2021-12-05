#include "pch.h"

#include <fstream>
#include <string>

#include "utilities.h"

DLLEXPORT std::vector<int> readInts(std::istream& os)
{
	std::vector<int> result;
	while (!os.eof())
	{
		int i;
		os >> i;
		result.push_back(i);
	}
	return result;
}

DLLEXPORT std::vector<std::string> readLines(const std::string& filePath)
{
	std::ifstream ifs(filePath);
	std::string line;
	std::vector<std::string> result;
	while (std::getline(ifs, line))
		result.push_back(line);
	return result;
}
