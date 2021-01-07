#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <stack>
#include <cassert>
#include <chrono>

const std::vector<uint32_t> EXAMPLE{28,33,18,42,31,14,46,20,48,47,24,23,49,45,19,38,39,11,1,32,25,35,8,17,7,9,4,2,34,10,3};

uint32_t validPathCount(uint32_t currentDepth, uint32_t lastNumber, const std::vector<uint32_t>& sequence)
{
	constexpr uint32_t MAX_DIFF(3);
	if (currentDepth < sequence.size() - 1)
	{
		uint32_t result(0);
		if (sequence[currentDepth + 1] - lastNumber <= MAX_DIFF)
			result += validPathCount(currentDepth + 1, lastNumber, sequence);
		if (sequence[currentDepth] - lastNumber <= MAX_DIFF)
			result += validPathCount(currentDepth + 1, sequence[currentDepth], sequence);
		return result;
	}
	else
		return 1;
}

uint32_t validPathCount(const std::vector<uint32_t>& sequence)
{
	assert(!sequence.empty());
	return validPathCount(1, sequence[0], sequence);
}

int main()
{
	std::ifstream is("input.txt");
	uint32_t n;
	std::vector<uint32_t> numbers;

	while (is >> n)
		numbers.push_back(n);

	//numbers = EXAMPLE;

	numbers.push_back(0);
	std::sort(std::begin(numbers), std::end(numbers));
	numbers.push_back(numbers[numbers.size() - 1] + 3);

	auto sequenceStart = std::begin(numbers);
	auto sequenceEnd = sequenceStart;

	std::vector<std::vector<uint32_t>> sequences;
	sequences.push_back({});
	auto currentSequence = &sequences[0];

	uint32_t gaps[4] = { 0,0,0,0 };
	for (size_t i = 0; i < numbers.size() - 1; ++i)
	{
		currentSequence->push_back(numbers[i]);
		auto diff = numbers[i + 1] - numbers[i];
		assert(diff >= 1 && diff <= 3);
		gaps[diff]++;

		if (diff == 3 && i < numbers.size() - 2)
		{
			sequences.push_back({});
			currentSequence = &sequences[sequences.size() - 1];
		}
	}

	uint64_t validPaths(1);
	for (auto& s : sequences)
		validPaths *= validPathCount(s);

	std::cout << "Day10 part 1: " << gaps[1] * gaps[3] << std::endl;
	std::cout << "Day10 part 2: " << validPaths << std::endl;

}
