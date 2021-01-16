// https://adventofcode.com/2019/day/16

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <functional>

const char* EXAMPLE1 = "19617804207202209144916044189917";

std::ostream& operator<<(std::ostream& os, const std::vector<uint8_t>& v)
{
	for (auto c : v)
		os << c;
	return os;
}


int main()
{
	//std::istringstream is(EXAMPLE1);
	std::ifstream is("input.txt");

	std::string input;
	std::getline(is, input);

	std::vector<uint8_t> list;
	list.reserve(input.size());
	for (auto c : input)
		list.push_back(c - '0');


	const size_t iterations = 100;
	std::vector basePattern = { 0, 1, 0, -1 };

	std::vector<uint8_t> listA(list.size());
	std::vector<uint8_t> listB(list.begin(), list.end());
	auto* current = &listA;
	auto* old = &listB;

	for (size_t iteration = 0; iteration < iterations; ++iteration)
	{
		for (size_t i = 0; i < current->size(); ++i)
		{
			int64_t newValue{ 0 };
			for (size_t j = 0; j < current->size(); ++j)
			{
				auto basePatternIndex = ((j + 1) / (i + 1)) % 4;
				newValue += int64_t((*old)[j]) * basePattern[basePatternIndex];
			}
			(*current)[i] = uint8_t(abs(newValue % 10));
		}
		std::swap(current, old);
	}

	std::vector<uint8_t> finalList{};
	std::transform(old->begin(), old->begin() + 8, std::back_inserter(finalList), [](uint8_t e) { return uint8_t(e + '0'); });

	std::cout << "Day16 Part1: " << finalList << std::endl;
	std::cout << "Day16 Part2: " << std::endl;
}
