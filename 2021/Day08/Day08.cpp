#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <fstream>
#include <iostream>
#include <numeric>
#include <ranges>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>


#include "../Utilities/utilities.h"

using namespace std::string_literals;
namespace ranges = std::ranges;

constexpr const size_t NUM_BITS = 7;
using bits_t = std::bitset<NUM_BITS>;

void readTokens(std::istream& is, auto& container, auto num)
{
	std::vector<std::string> input;
	for (size_t i = 0; i < num; ++i)
	{
		std::string token;
		is >> token;
		if (token == "")
			return;
		input.push_back(std::move(token));
	}
	container.push_back(std::move(input));
};

[[nodiscard]]
auto makeBitset(const std::string_view& pattern) -> bits_t
{
	return std::accumulate(begin(pattern), end(pattern), bits_t{}, [](auto bits, char ch)
	{
		bits.set(ch - 'a', true);
		return bits;
	});
}

template<class Container, typename T = Container::value_type>
[[nodiscard]]
auto pop_if(Container& container, auto pred) -> Container::value_type
{
	auto it = ranges::find_if(container, pred);
	T result = *it;
	container.erase(it);
	return result;

}

int main()
{
	std::ifstream ifs("input.txt");
	
	std::vector<std::vector<std::string>> inputs;
	std::vector<std::vector<std::string>> outputs;

	while (ifs)
	{
		readTokens(ifs, inputs, 10);
		std::string _; ifs >> _; // discard |
		readTokens(ifs, outputs, 4);
	}

	auto count = std::accumulate(begin(outputs), end(outputs), 0ll, [](auto acc, auto& output)
	{
		return acc + std::count_if(begin(output), end(output), [](auto& str) 
		{
			auto len = str.length();
			return len == 2 || len == 3 || len == 4 || len == 7;
		});
	});

	int outSum{ 0 };
	for (size_t i = 0; i < inputs.size(); ++i)
	{
		std::vector<bits_t> inputBits, outputBits;
		ranges::transform(inputs[i], std::back_inserter(inputBits), makeBitset);
		ranges::transform(outputs[i], std::back_inserter(outputBits), makeBitset);

		std::array<bits_t, 10> bitpatterns{};

		// Calculate which patterns corresponds to which number
		bitpatterns[1] = pop_if(inputBits, [](auto& bits) { return bits.count() == 2; });
		bitpatterns[7] = pop_if(inputBits, [](auto& bits) { return bits.count() == 3; });
		bitpatterns[4] = pop_if(inputBits, [](auto& bits) { return bits.count() == 4; });
		bitpatterns[8] = pop_if(inputBits, [](auto& bits) { return bits.count() == 7; });

		bitpatterns[6] = pop_if(inputBits, [&](auto& bits) { return bits.count() == 6 && (bitpatterns[1] & ~bits).any(); });
		bitpatterns[0] = pop_if(inputBits, [&](auto& bits) { return bits.count() == 6 && (bitpatterns[4] & ~bits).any(); });
		bitpatterns[9] = pop_if(inputBits, [&](auto& bits) { return bits.count() == 6; });
		
		bitpatterns[5] = pop_if(inputBits, [&](auto& bits) { return bits.count() == 5 && (bits | bitpatterns[1]) == bitpatterns[9]; });
		bitpatterns[3] = pop_if(inputBits, [&](auto& bits) { return bits.count() == 5 && (bits & bitpatterns[1]) == bitpatterns[1]; });
		bitpatterns[2] = inputBits.front();
		assert(inputBits.size() == 1);

		// Match output patterns to the actual number
		int outNumber = std::accumulate(outputBits.begin(), outputBits.end(), 0, [multiplier = 1000, &bitpatterns](auto number, auto bits) mutable 
		{
			int digit = 0;
			[[maybe_unused]] // don't really care about the result, all we want is to increment digit until the pattern is found
			auto _ = std::any_of(bitpatterns.begin(), bitpatterns.end(), [&](auto pattern) { return bits == pattern ? true : (++digit, false); });

			number += digit * multiplier;
			multiplier /= 10;
			return number;
		});

		outSum += outNumber;
	}

	std::cout << "Day08 Part 1: " << count << '\n';
	std::cout << "Day08 Part 2: " << outSum << '\n';
}
