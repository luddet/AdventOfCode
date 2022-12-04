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

	auto part1 = std::count_if(cbegin(lines), cend(lines),[](const auto& line) {
		std::array<int, 4> n{};
		std::regex r(R"(\d+)");
		std::transform(std::sregex_token_iterator(begin(line), end(line), r), std::sregex_token_iterator(), begin(n), [](const std::string& match)
		{
			return std::atoi(match.c_str());
		});
		return n[0] <= n[2] && n[1] >= n[3] || n[0] >= n[2] && n[1] <= n[3];
	});


	std::cout << "Day04 Part 1: " << part1 << '\n';
	std::cout << "Day04 Part 2: " << '\n';
}
