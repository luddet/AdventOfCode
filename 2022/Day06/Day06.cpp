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

int main()
{
	auto input = getInput(2022, 6);
	//input = "zcfzfwzzqfrljwzlrfnpqdbhtmscgvjw";
	int part1{ 0 };
	constexpr static int windowSize{ 4 };
	for (const auto& window : input | views::slide(windowSize))
	{
		std::set<char> unique{};
		std::unique_copy(window.begin(), window.end(), std::inserter(unique, unique.end()));
		if (unique.size() == 4)
			break;
		++part1;
	}
	part1 += windowSize;

	std::cout << "Day06 Part 1: " << part1 << '\n';
	//std::cout << "Day06 Part 2: " << '\n';
}
