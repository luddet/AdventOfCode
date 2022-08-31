#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>
#include <concepts>
#include <ranges>

#include "../Utilities/utilities.h"

using std::ranges::sized_range;

namespace
{
constexpr size_t NUM_BITS(12);
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
	std::vector<uint16_t> numbers(lines.size());
	for (auto& line : lines)
		numbers.push_back(std::stoi(line, nullptr, 2));

	uint32_t numOnes[NUM_BITS]{ 0,0,0,0,0,0,0,0,0,0,0,0 };
	uint32_t numZeros[NUM_BITS]{ 0,0,0,0,0,0,0,0,0,0,0,0 };

	for (auto n : numbers)
	{
		for (size_t bit = 0; bit < NUM_BITS; ++bit)
		{
			if ((n & (1 << bit)) != 0)
				++numOnes[bit];
			else
				++numZeros[bit];
		}
	}

	uint16_t o2_criteria{ 0 };
	uint16_t co2_criteria{ 0 };

	for (size_t bit = 0; bit < NUM_BITS; ++bit)
	{
		o2_criteria |= (numOnes[bit] >= numZeros[bit]) ? 1 << bit : 0;
		co2_criteria |= (numOnes[bit] < numZeros[bit]) ? 1 << bit : 0;
	}

	auto o2nums{ numbers };
	for (size_t bit = 0; bit < NUM_BITS; ++bit)
	{
		uint16_t bitMask = o2_criteria & (1 << bit);
		uint16_t numErased{ 0 };
		std::erase_if(o2nums, [&o2nums, bitMask, &numErased](uint16_t n) {
			bool erase = (n & bitMask) == 0 && o2nums.size() > numErased + 1;
			if (erase)
				++numErased;
			return erase;
		});
		if (o2nums.size() == 1)
			break;
	}

	auto co2nums{ numbers };
	for (size_t bit = 0; bit < NUM_BITS; ++bit)
	{
		uint16_t bitMask = co2_criteria & (1 << bit);
		std::erase_if(co2nums, [&co2nums, bitMask](uint16_t n) {
			return (n & bitMask) == 0 && co2nums.size() > 1;
		});
		if (co2nums.size() == 1)
			break;
	}

	return o2nums[0] * co2nums[0];
}


int main()
{
	auto lines{ readLines("input.txt") };
	std::cout << "Day03 part 1: " << part1(lines) << std::endl;
	std::cout << "Day03 part 2: " << part2(lines) << std::endl;
}
