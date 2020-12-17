#include <iostream>
#include <vector>
#include <unordered_map>


const std::vector<uint64_t> EXAMPLE1{ 0, 3, 6 };

const std::vector<uint64_t> INPUT{ 11,0,1,10,5,19 };

uint64_t execute(const std::vector<uint64_t>& input, uint64_t turns)
{
	// <number, <lastTurn, wasFirstTime>>
	std::unordered_map<uint64_t, std::pair<uint64_t, bool>> map;
	uint64_t lastNumberSpoken(0);

	size_t currentTurn;
	uint64_t currentNumberToSpeak;
	for (currentTurn = 0; currentTurn < input.size(); ++currentTurn)
	{
		lastNumberSpoken = input[currentTurn];
		map[lastNumberSpoken] = { currentTurn, true };
	}

	for (; currentTurn < turns; ++currentTurn)
	{
		auto it = map.find(lastNumberSpoken);
		auto [lastTurn, lastFirstTime] = it->second;
		it->second = { currentTurn - 1, false };

		if (lastFirstTime)
			currentNumberToSpeak = 0;
		else
			currentNumberToSpeak = currentTurn - 1 - lastTurn;

		if (map.find(currentNumberToSpeak) == map.end())
			map[currentNumberToSpeak] = { currentTurn, true };
		else
			map[currentNumberToSpeak].second = false;

		lastNumberSpoken = currentNumberToSpeak;
	}
	return lastNumberSpoken;
}

int main()
{
	auto& input = INPUT;

	auto part1 = execute(input, 2020);
	auto part2 = execute(input, 30000000);

	std::cout << "Day15 Part 1: " << part1 << std::endl;
	std::cout << "Day15 Part 2: " << part2 << std::endl;
}
