#include <algorithm>
#include <array>
#include <bitset>
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

constexpr size_t NUM_BITS = 7;
using bits_t = std::bitset<NUM_BITS>;

auto readTokens(std::istream& is, auto& container, auto num)
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

[[nodiscard]] bits_t makeBitset(const std::string_view& pattern)
{
	return std::accumulate(begin(pattern), end(pattern), bits_t{}, [](auto bits, char ch)
	{
		bits.set(ch - 'a', true);
		return bits;
	});
}

template<class Container>
auto pop_if(Container& container, auto pred) -> Container::value_type
{
	auto it = ranges::find_if(container, pred);
	Container::value_type result = *it;
	container.erase(it);
	return result;

}

int main()
{
	std::ifstream ifs("test_input.txt");
	
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

	for (size_t i = 0; i < inputs.size(); ++i)
	{
		std::vector<bits_t> inputBits, outputBits;
		
		ranges::transform(inputs[i], std::back_inserter(inputBits), makeBitset);
		ranges::transform(outputs[i], std::back_inserter(outputBits), makeBitset);

		std::array<bits_t, 10> bitpatterns{};

		bitpatterns[1] = pop_if(inputBits, [](auto& bits) { return bits.count() == 2; });
		bitpatterns[7] = pop_if(inputBits, [](auto& bits) { return bits.count() == 3; });
		bitpatterns[4] = pop_if(inputBits, [](auto& bits) { return bits.count() == 4; });
		bitpatterns[8] = pop_if(inputBits, [](auto& bits) { return bits.count() == 7; });

	}


	std::cout << "Day08 Part 1: " << count << '\n';
	std::cout << "Day08 Part 2: " << '\n';
}
