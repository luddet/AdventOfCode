#include <algorithm>
#include <fstream>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>

#include "../Utilities/utilities.h"

namespace ranges = std::ranges;

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

	int totalRiskLevel = 0;

	// check corners
	{
		size_t i = 0; // top left
		if (grid[i] < grid[i + 1] && grid[i] < grid[i + width])
			totalRiskLevel += grid[i] + 1;

		i = width - 1; // top right
		if (grid[i] < grid[i - 1] && grid[i] < grid[i + width])
			totalRiskLevel += grid[i] + 1;

		i = (height - 1) * width; // bottom left
		if (grid[i] < grid[i + 1] && grid[i] < grid[i - width])
			totalRiskLevel += grid[i] + 1;

		i = grid.size() - 1; // bottom right
		if (grid[i] < grid[i - 1] && grid[i] < grid[i - width])
			totalRiskLevel += grid[i] + 1;
	}

	// check top
	for (size_t i = 1; i < width - 1; ++i)
		if (grid[i] < grid[i - 1] && grid[i] < grid[i + width] && grid[i] < grid[i + 1])
			totalRiskLevel += grid[i] + 1;

	// check left 
	for (size_t i = width; i < grid.size()-width; i += width)
		if (grid[i] < grid[i - width] && grid[i] < grid[i + 1] && grid[i] < grid[i + width])
			totalRiskLevel += grid[i] + 1;

	// check right
	for (size_t i = 2*width-1; i < grid.size()-width; i += width)
		if (grid[i] < grid[i - width] && grid[i] < grid[i - 1] && grid[i] < grid[i + width])
			totalRiskLevel += grid[i] + 1;

	// check bottom
	for (size_t i = grid.size() - width + 1; i < grid.size() - 1; ++i)
		if (grid[i] < grid[i - 1] && grid[i] < grid[i - width] && grid[i] < grid[i + 1])
			totalRiskLevel += grid[i] + 1;

	// check interior
	for (size_t row = 1; row < height - 1; ++row)
		for (size_t col = 1; col < width - 1; ++col)
		{
			auto i = row * width + col;
			if (grid[i] < grid[i - width] && grid[i] < grid[i + width] && grid[i] < grid[i - 1] && grid[i] < grid[i + 1])
				totalRiskLevel += grid[i] + 1;
		}


	std::cout << "Day09 Part 1: " << totalRiskLevel << '\n';
	std::cout << "Day09 Part 2: " << '\n';
}
