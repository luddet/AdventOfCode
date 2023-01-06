#include <algorithm>
#include <iostream>
#include <numeric>
#include <ranges>
#include <string>
#include <vector>

#include "../../Utilities/Utilities/utilities.h"

namespace views = std::views;
namespace ranges = std::ranges;

auto accumulate(auto r, auto init) -> decltype(init) { return std::accumulate(std::begin(r), std::end(r), init); }
bool empty_lines(const std::string& s1, const std::string&) { return s1.size() != 0; }
bool non_empty(const std::string& str) { return str.size() > 0; }

int str_to_i(const std::string& str) { return atoi(str.c_str()); }
constexpr static auto sum_integer_strings = [](const auto chunk_view) { return accumulate(chunk_view | views::filter(non_empty) | views::transform(str_to_i), 0); };

int main()
{
	std::vector<std::string> input{ readLines("input.txt")};
	std::vector<int> summed_calories = input | views::chunk_by(empty_lines) | views::transform(sum_integer_strings) | ranges::to<std::vector>();
	ranges::sort(summed_calories, ranges::greater());

	std::cout << "Day01 Part 1: " << summed_calories.front() << '\n';
	std::cout << "Day01 Part 2: " << accumulate(summed_calories | views::take(3), 0) << '\n';
}
