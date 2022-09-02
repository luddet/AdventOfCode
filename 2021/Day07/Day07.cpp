#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <vector>

#include "../Utilities/utilities.h"
namespace ranges = std::ranges;


int main()
{
	std::istringstream test("16,1,2,0,4,2,7,1,2,14");
	std::ifstream ifs("input.txt");
	auto positions = readItems<int64_t>(ifs, ',');

	std::vector<std::pair<int64_t, int64_t>> positionMoves;
	auto [min, max] = ranges::minmax_element(positions);
	for (int64_t i = *min; i <= *max; ++i)
	{
		auto numMoves = std::accumulate(begin(positions), end(positions), 0ll, [i](int64_t moves, int64_t pos) { return moves + std::abs(i - pos); });
		positionMoves.push_back({ i, numMoves });
		std::push_heap(begin(positionMoves), end(positionMoves), [](auto& a, auto& b) { return a.second > b.second; });
	}

	auto [pos, moves] = positionMoves.front();

	std::cout << "Day07 Part 1: pos: " << pos << ", moves: " << moves << '\n';
	std::cout << "Day07 Part 2: " << '\n';
}
