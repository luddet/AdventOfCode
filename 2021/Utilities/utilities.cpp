//#include "pch.h"

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
	return readLines(std::ifstream(filePath));
}

DLLEXPORT std::vector<std::string> readLines(std::istream& is)
{
	std::vector<std::string> result;
	for (std::string line; std::getline(is, line);)
		result.push_back(line);
	return result;

}

DLLEXPORT std::vector<std::string> readLines(std::istream&& is)
{
	return readLines(is);
}