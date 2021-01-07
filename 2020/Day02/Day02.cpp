#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>

bool isEntryValidPart1(std::string& entry)
{
	std::istringstream entryStream(entry);
	int minLimit, maxLimit;
	char limitedChar, skip;
	std::string password;

	entryStream >> std::skipws >> minLimit >> skip >> maxLimit >> limitedChar >> skip >> password;

	uint8_t limitedCharCount = std::count(std::begin(password), std::end(password), limitedChar);

	return limitedCharCount >= minLimit && limitedCharCount <= maxLimit;
}

bool isEntryValidPart2(std::string& entry)
{
	std::istringstream entryStream(entry);
	int index1, index2;
	char limitedChar, skip;
	std::string password;

	entryStream >> std::skipws >> index1 >> skip >> index2 >> limitedChar >> skip >> password;

	--index1;
	--index2;

	return (password[index1] == limitedChar) ^ (password[index2] == limitedChar);
}

int main()
{
	std::vector<std::pair<std::string, bool>> testInputPart1
	{
		std::make_pair("1 - 3 a: abcde", true),
		std::make_pair("1 - 3 b: cdefg", false),
		std::make_pair("2 - 9 c: ccccccccc", true),
		std::make_pair("2 - 4 b: bbbbb", false),
		std::make_pair("2 - 3 d: ada", false),
	};

	std::vector<std::pair<std::string, bool>> testInputPart2
	{
		std::make_pair("1 - 3 a: abcde", true),
		std::make_pair("1 - 3 b: cdefg", false),
		std::make_pair("2 - 9 c: ccccccccc", false),
	};

	for (auto& p : testInputPart1)
		assert(isEntryValidPart1(p.first) == p.second);

	for (auto& p : testInputPart2)
		assert(isEntryValidPart2(p.first) == p.second);

	std::ifstream ifs("input.txt");
	std::string line;
	int validCountPart1 = 0;
	int validCountPart2 = 0;

	while (std::getline(ifs, line))
	{
		if (isEntryValidPart1(line))
			++validCountPart1;
		if (isEntryValidPart2(line))
			++validCountPart2;

	}

	std::cout << "Day02 Part 1: " << validCountPart1 << std::endl;
	std::cout << "Day02 Part 2: " << validCountPart2 << std::endl;
}
