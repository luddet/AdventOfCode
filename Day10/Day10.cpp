#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <stack>
#include <cassert>
#include <chrono>

const std::vector<int32_t> EXAMPLE{28,33,18,42,31,14,46,20,48,47,24,23,49,45,19,38,39,11,1,32,25,35,8,17,7,9,4,2,34,10,3};



uint32_t validPathCount(uint32_t currentDepth, int32_t lastNumber, const std::vector<int32_t>& sequence)
{
	constexpr uint32_t MAX_DIFF(3);
	uint32_t result(0);
	if (currentDepth == sequence.size() - 1)
	{
		return 1;
		//result = (sequence[currentDepth] - lastNumber <= MAX_DIFF) ? 1 : 0;
	}
	else
	{
		if (sequence[currentDepth] - lastNumber <= MAX_DIFF)
			result = validPathCount(currentDepth + 1, sequence[currentDepth], sequence);
		if (sequence[currentDepth + 1] - lastNumber <= MAX_DIFF)
			result += validPathCount(currentDepth + 1, lastNumber, sequence);
	}
	return result;
}

uint32_t validPathCount(const std::vector<int32_t>& sequence)
{
	return validPathCount(1, 0, sequence);
}

int main()
{
	std::ifstream is("input.txt");
	int32_t n;
	std::vector<int32_t> numbers;

	while (is >> n)
		numbers.push_back(n);

	numbers = EXAMPLE;

	numbers.push_back(0);
	std::sort(std::begin(numbers), std::end(numbers));
	numbers.push_back(numbers[numbers.size() - 1] + 3);

	uint32_t oneGaps(0), threeGaps(0);
	for (size_t i = 0; i < numbers.size() - 1; ++i)
	{
		switch (numbers[i + 1] - numbers[i])
		{
			case 1:
				oneGaps++;
				break;
			case 3:
				threeGaps++;
				break;
		}
	}
	auto part1 = oneGaps * threeGaps;

	//numbers = { (0), 1, 4, 5, 6, 7, 10, 11, 12, 15, 16, 19, (22) };

	// DEBUG
	//std::vector<int32_t> temp;
	//for (size_t i = 0; i < 70 && i < numbers.size(); ++i)
	//	temp.push_back(numbers[i]);
	//temp.push_back(temp[temp.size() - 1] + 3);
	//numbers = temp;
	// END DEBUG

	auto startTime = std::chrono::steady_clock::now();
	uint32_t validPaths = validPathCount(numbers);
	auto endTime = std::chrono::steady_clock::now();
	std::chrono::duration<double> timeTaken = (endTime - startTime);

	std::cout << "Day10 part 1: " << part1 << std::endl;
	std::cout << "Day10 part 2: " << validPaths << ", time taken: " << timeTaken.count() << " s." << std::endl;

}
