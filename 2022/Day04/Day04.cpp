#include <iostream>
#include <string>
#include <array>
#include <algorithm>
#include <regex>

#include "../../Utilities/Utilities/utilities.h"

int main()
{
	auto lines{ readLines("input.txt") };

	const std::regex pattern(R"((\d+)-(\d+),(\d+)-(\d+))");
	int part1{}, part2{};

	for(const auto& line : lines)
	{
		std::array<int, 4> n{};
		std::smatch m;
		std::regex_match(line, m, pattern);
		std::transform(m.begin() + 1, m.end(), n.begin(), [](auto submatch) { return std::stoi(submatch.str()); });

		if (n[0] <= n[2] && n[1] >= n[3] || n[0] >= n[2] && n[1] <= n[3])
			++part1;

		if (n[0] <= n[2] && n[1] >= n[2] || n[0] <= n[3] && n[1] >= n[3] || n[2] <= n[0] && n[3] >= n[0] || n[2] <= n[1] && n[3] >= n[1])
			++part2;
	}

	std::cout << "Day04 Part 1: " << part1 << '\n';
	std::cout << "Day04 Part 2: " << part2 << '\n';
}
