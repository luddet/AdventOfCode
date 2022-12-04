#include <fstream>
#include <iostream>
#include <cstring>
#include <string>
#include <string_view>
#include <array>
#include <algorithm>
#include <numeric>
#include <ranges>
#include <regex>

#include "../../2021/Utilities/utilities.h"

int main()
{
	auto lines{ readLines("input.txt") };

	int part1{}, part2{};
	std::array<int, 4> n{};
	const std::regex r(R"(\d+)");

	for(const auto& line : lines)
	{
		std::transform(std::sregex_token_iterator(begin(line), end(line), r), std::sregex_token_iterator(), begin(n), [](const std::string& match)
		{
			return std::atoi(match.c_str());
		});

		// full overlap
		if (n[0] <= n[2] && n[1] >= n[3] || n[0] >= n[2] && n[1] <= n[3])
			++part1;

		// some overlap
		if (n[0] <= n[2] && n[1] >= n[2] || n[0] <= n[3] && n[1] >= n[3] || n[2] <= n[0] && n[3] >= n[0] || n[2] <= n[1] && n[3] >= n[1])
			++part2;
	}

	std::cout << "Day04 Part 1: " << part1 << '\n';
	std::cout << "Day04 Part 2: " << part2 << '\n';
}
