#include <iostream>
#include <string>
#include <array>
#include <algorithm>
#include <regex>

#include "../../2021/Utilities/utilities.h"

int main()
{
	auto lines{ readLines("input.txt") };

	const std::regex delimiters("[-,]");
	int part1{}, part2{};

	for(const auto& line : lines)
	{
		std::array<int, 4> n{};
		std::transform(std::sregex_token_iterator(begin(line), end(line), delimiters, -1), std::sregex_token_iterator(), begin(n), [](const std::string& match)
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
