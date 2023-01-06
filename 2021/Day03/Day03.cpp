#include <algorithm>
#include <cassert>
#include <execution>
#include <iostream>
#include <vector>
#include <concepts>
#include <ranges>
#include <numeric>

#include "../../Utilities/Utilities/utilities.h"

using std::ranges::sized_range;

namespace
{
constexpr auto NUM_BITS{ 12U };
}

int part1(const std::vector<std::string>& lines)
{
	uint32_t numOnes[NUM_BITS]{ 0,0,0,0,0,0,0,0,0,0,0,0 };

	for (auto& line : lines)
	{
		assert(line.length() == NUM_BITS);
		for (size_t i = 0; i < line.length(); ++i)
		{
			if (line[i] == '1')
				++numOnes[i];
		}
	}

	uint16_t gamma{ 0 };
	size_t numLines(lines.size());
	for (size_t i = 0; i < NUM_BITS; ++i)
	{
		if (numOnes[i] > numLines - numOnes[i])
			gamma |= 1 << (NUM_BITS - i - 1);
	}
	uint16_t epsilon = ~gamma & 0b0000111111111111;
	return epsilon * gamma;
}

int part2(const sized_range auto& lines)
{
	std::vector<uint16_t> numbers;
	std::transform(begin(lines), end(lines), std::back_inserter(numbers), [](auto line) { return std::stoi(line, nullptr, 2); });

	auto calculate = [numBits = lines.front().length()](const auto& numbers, auto criteria) -> uint16_t
	{
		auto workingSet{ numbers };
		auto currentBit = numBits;
		while (workingSet.size() > 1 && currentBit > 0)
		{
			std::vector<uint16_t> ones;
			std::vector<uint16_t> zeros;

			--currentBit;
			std::for_each(begin(workingSet), end(workingSet), [=, &ones, &zeros](auto n)
			{
				if (((n >> currentBit) & 1) == 1)
					ones.push_back(n);
				else
					zeros.push_back(n);
			});

			workingSet = criteria(ones.size(), zeros.size()) ? std::move(ones) : std::move(zeros);
		}
		assert(workingSet.size() == 1);
		return workingSet.front();
	};

	auto oxygenGeneratorRating = calculate(numbers, [](auto ones, auto zeros) {return ones >= zeros; });
	auto co2ScrubberRating = calculate(numbers, [](auto ones, auto zeros) {return ones < zeros; });

	return oxygenGeneratorRating * co2ScrubberRating;
}


int main()
{
	auto lines{ readLines("input.txt") };
	std::cout << "Day03 part 1: " << part1(lines) << std::endl;
	std::cout << "Day03 part 2: " << part2(lines) << std::endl;
}
