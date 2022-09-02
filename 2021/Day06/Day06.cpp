#include <algorithm>
#include <array>
#include <concepts>
#include <fstream>
#include <iostream>
#include <numeric>
#include <ranges>
#include <sstream>

#include "../Utilities/utilities.h"

namespace ranges = std::ranges;

std::ostream& printTimers(std::ostream& os, const ranges::sized_range auto& timers)
{
	auto numberOfFish = std::accumulate(begin(timers), end(timers), 0);
	os << "Total: " << numberOfFish << ": ";
	for (size_t i = 0; i < timers.size(); ++i)
		os << timers[i] << ", ";
	return os;
}

int main()
{
	std::ios::sync_with_stdio(false);

	std::ifstream ifs("input.txt");
	std::istringstream testdata("3,4,3,1,2");

	auto input = readItems<int>(ifs,',');

	constexpr size_t cycleLength = 7;
	constexpr size_t maxDays = 9;

	std::array<int, maxDays> timers{0};

	for (auto n : input)
		++timers[n];

	const size_t daysToSimulate = 80;
	for (auto n = 0; n < daysToSimulate; ++n)
	{
		auto spawns = timers[0];
		for (size_t i = 0; i < timers.size() - 1; ++i)
			timers[i] = timers[i + 1];
		timers[cycleLength - 1] += spawns;
		timers[timers.size() - 1] = spawns;
		printTimers(std::cout, timers) << '\n';
	}

	auto numberOfFish = std::accumulate(begin(timers), end(timers), 0);
	std::cout << "Day06 Part 1: " << numberOfFish << '\n';
	std::cout << "Day06 Part 2: " << '\n';
}
