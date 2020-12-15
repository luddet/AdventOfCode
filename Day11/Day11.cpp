#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

const std::string EXAMPLE =
"L.LL.LL.LL\n"
"LLLLLLL.LL\n"
"L.L.L..L..\n"
"LLLL.LL.LL\n"
"L.LL.LL.LL\n"
"L.LLLLL.LL\n"
"..L.L.....\n"
"LLLLLLLLLL\n"
"L.LLLLLL.L\n"
"L.LLLLL.LL";

using grid_t = std::vector<char>;

uint32_t countInNeighbourhood(grid_t& grid, int32_t index, int32_t numRows, int32_t numCols, char counted)
{
	size_t row(index / numCols), col(index % numCols);
	int32_t count(0);
	if (row != 0 && col != 0 && grid[row * numCols + col - numCols - 1] == counted) // TOP LEFT
		++count;
	if (row != 0 && grid[row * numCols + col - numCols] == counted) // TOP
		++count;
	if (row != 0 && col != numCols - 1 && grid[row * numCols + col - numCols + 1] == counted) // TOP RIGHT
		++count;
	if (col != 0 && grid[row * numCols + col - 1] == counted) // LEFT
		++count;
	if (col != numCols - 1 && grid[row * numCols + col + 1] == counted) // RIGHT
		++count;
	if (row != numRows - 1 && col != 0 && grid[row * numCols + col + numCols - 1] == counted) // BOTTOM LEFT
		++count;
	if (row != numRows - 1 && grid[row * numCols + col + numCols] == counted) // BOTTOM
		++count;
	if (row != numRows - 1 && col != numCols - 1 && grid[row * numCols + col + numCols + 1] == counted) // BOTTOM RIGHT
		++count;
	return count;
}

std::ostream& printGrid(std::ostream& os, const grid_t& grid, int32_t numCols)
{
	for (size_t i = 0; i < grid.size(); ++i)
	{
		os << grid[i];
		if ((i % numCols) == numCols - 1)
			os << std::endl;
	}
	return os;
}

int main()
{
	using std::begin;
	using std::end;

	//std::istringstream is(EXAMPLE);
	std::ifstream is("input.txt");
	grid_t grid;
	int32_t numRows(0), numCols(0);
	std::string line;
	while (std::getline(is, line) && !line.empty())
	{
		++numRows;
		numCols = line.length();
		std::copy(begin(line), end(line), std::back_inserter(grid));
	}
	

	uint32_t iteration(0);
	//std::cout << "iteration: " << iteration << std::endl;
	//printGrid(std::cout, grid, numCols) << std::endl;

	bool changed = true;
	while (changed)
	{
		grid_t oldGrid(begin(grid), end(grid));
		changed = false;
		for (size_t i = 0; i < grid.size(); ++i)
		{
			switch ((oldGrid)[i])
			{
				case 'L':
					if (countInNeighbourhood((oldGrid), i, numRows, numCols, '#') == 0)
					{
						(grid)[i] = '#';
						changed = true;
					}
					break;
				case '#':
					if (countInNeighbourhood((oldGrid), i, numRows, numCols, '#') >= 4)
					{
						(grid)[i] = 'L';
						changed = true;
					}
					break;
			}
		}

		std::cout << "iteration: " << ++iteration << std::endl;
		//printGrid(std::cout, grid, numCols) << std::endl;
		//std::getline(std::cin, line);
	}

	auto occupiedCount = std::count(begin(grid), end(grid), '#');
	std::cout << "Day11 Part 1: " << occupiedCount << std::endl;

}
