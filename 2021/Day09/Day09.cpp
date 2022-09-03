#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <numeric>
#include <ranges>
#include <stack>
#include <string>
#include <unordered_set>
#include <vector>

#include "../Utilities/utilities.h"

namespace ranges = std::ranges;

struct Basin
{
	size_t index{ 0 };
	uint8_t height{ 0 };
	size_t size{ 0 };
};


auto findSize(size_t startIndex, const std::vector<uint8_t>& grid, size_t gridWidth, uint8_t boundaryValue) -> size_t
{
	std::unordered_set<size_t> visited, unvisited;

	auto addToUnvisited = [&](size_t i)
	{
		if (grid[i] < boundaryValue && visited.find(i) == visited.end())
			unvisited.insert(i);
	};

	size_t size{ 0 };
	unvisited.insert(startIndex);
	while (!unvisited.empty())
	{
		auto current = *unvisited.begin();
		unvisited.erase(current);
		visited.insert(current);

		++size;

		// left
		if (current % gridWidth != 0)
			addToUnvisited(current - 1);

		// right 
		if ((current + 1) % gridWidth != 0)
			addToUnvisited(current + 1);

		// top
		if (current >= gridWidth)
			addToUnvisited(current - gridWidth);

		// bottom
		if (current < grid.size() - gridWidth)
			addToUnvisited(current + gridWidth);
	}
	return size;
}


int main()
{
	std::ifstream ifs("input.txt");
	std::string line;
	std::getline(ifs, line);

	const size_t width = line.length();
	ifs.seekg(0);
	auto grid = readItems<uint8_t>(ifs);
	ranges::for_each(grid, [](auto& c) { c -= '0'; });

	const size_t height = grid.size() / width;

	std::vector<Basin> basins;
	// check corners
	{
		size_t i = 0; // top left
		if (grid[i] < grid[i + 1] && grid[i] < grid[i + width])
			basins.push_back({ i, grid[i] });

		i = width - 1; // top right
		if (grid[i] < grid[i - 1] && grid[i] < grid[i + width])
			basins.push_back({ i, grid[i] });

		i = (height - 1) * width; // bottom left
		if (grid[i] < grid[i + 1] && grid[i] < grid[i - width])
			basins.push_back({ i, grid[i] });

		i = grid.size() - 1; // bottom right
		if (grid[i] < grid[i - 1] && grid[i] < grid[i - width])
			basins.push_back({ i, grid[i] });
	}

	// check top
	for (size_t i = 1; i < width - 1; ++i)
		if (grid[i] < grid[i - 1] && grid[i] < grid[i + width] && grid[i] < grid[i + 1])
			basins.push_back({ i, grid[i] });

	// check left 
	for (size_t i = width; i < grid.size() - width; i += width)
		if (grid[i] < grid[i - width] && grid[i] < grid[i + 1] && grid[i] < grid[i + width])
			basins.push_back({ i, grid[i] });

	// check right
	for (size_t i = 2 * width - 1; i < grid.size() - width; i += width)
		if (grid[i] < grid[i - width] && grid[i] < grid[i - 1] && grid[i] < grid[i + width])
			basins.push_back({ i, grid[i] });

	// check bottom
	for (size_t i = grid.size() - width + 1; i < grid.size() - 1; ++i)
		if (grid[i] < grid[i - 1] && grid[i] < grid[i - width] && grid[i] < grid[i + 1])
			basins.push_back({ i, grid[i] });

	// check interior
	for (size_t row = 1; row < height - 1; ++row)
	{
		for (size_t col = 1; col < width - 1; ++col)
		{
			auto i = row * width + col;
			if (grid[i] < grid[i - width] && grid[i] < grid[i + width] && grid[i] < grid[i - 1] && grid[i] < grid[i + 1])
				basins.push_back({ i, grid[i] });
		}
	}

	auto totalRiskLevel = std::accumulate(basins.begin(), basins.end(), 0, [](auto acc, const auto& basin) { return acc + basin.height + 1; });

	for (auto& basin : basins)
		basin.size = findSize(basin.index, grid, width, 9);

	ranges::sort(basins, [](auto& lhs, auto& rhs) { return lhs.size > rhs.size; });
	auto part2 = std::accumulate(basins.begin(), basins.begin() + 3, 1ull, [](auto acc, auto& basin) { return acc * basin.size; });

	std::cout << "Day09 Part 1: " << totalRiskLevel << '\n';
	std::cout << "Day09 Part 2: " << part2 << '\n';
}
