#include <algorithm>
#include <concepts>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <vector>

#include "../../Utilities/Utilities/utilities.h"

namespace ranges = std::ranges;

auto calculate(const auto& positions, auto costFunction)
{
	std::vector<std::pair<int64_t, int64_t>> positionMoves;
	auto [min, max] = ranges::minmax_element(positions);
	for (int64_t i = *min; i <= *max; ++i)
	{
		auto numMoves = std::accumulate(begin(positions), end(positions), 0ll, [i, &costFunction](int64_t acc, int64_t pos) { return acc + costFunction(std::abs(i - pos)); });
		positionMoves.push_back({ i, numMoves });
		std::push_heap(begin(positionMoves), end(positionMoves), [](auto& a, auto& b) { return a.second > b.second; });
	}

	return positionMoves.front();
}

int main()
{
	std::istringstream test("16,1,2,0,4,2,7,1,2,14");
	std::ifstream ifs("input.txt");
	auto positions = readItems<int64_t>(ifs, ',');

	auto [linearCostPos, linearCost] = calculate(positions, [](auto steps) { return steps; });
	auto [varCostPos, varCost] = calculate(positions, [](auto steps) {
		int64_t cost{ 0 };
		for (int64_t i = 0; i <= steps; ++i)
			cost += i;
		return cost; 
	});

	std::cout << "Day07 Part 1: pos: " << linearCostPos << ", cost: " << linearCost << '\n';
	std::cout << "Day07 Part 2: pos: " << varCostPos << ", cost: " << varCost << '\n';
}
