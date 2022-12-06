#include <fstream>
#include <iostream>
#include <exception>
#include <ranges>
#include <set>
#include <algorithm>
#include <iterator>
#include <vector>

#include "../../2021/Utilities/utilities.h"

namespace views = std::views;

int findUniqueWindow(const std::ranges::range auto& input, const int windowSize)
{
	int index{ 0 };
	for (const auto& window : input | views::slide(windowSize))
	{
		std::set<char> unique;
		std::unique_copy(window.begin(), window.end(), std::inserter(unique, unique.end()));
		if (unique.size() == windowSize)
			break;
		++index;
	}
	return index + windowSize;
}

int main()
{
	auto input = getInput(2022, 6);
	int part1 = findUniqueWindow(input, 4);
	int part2 = findUniqueWindow(input, 14);
	
	std::cout << "Day06 Part 1: " << part1 << '\n';
	std::cout << "Day06 Part 2: " << part2 << '\n';
}
