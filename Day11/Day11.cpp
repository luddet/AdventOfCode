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

const char FREE('L'), OCCUPIED('#');

using grid_t = std::vector<std::string>;

enum class NeighbourhoodType
{
	Nearest,
	LineOfSight
};

uint32_t countOccupiedInNeighbourhood(grid_t& grid, size_t row, size_t col, NeighbourhoodType type)
{
	uint32_t count(0);
	uint32_t maxSteps = (type == NeighbourhoodType::Nearest) ? 1 : UINT32_MAX;

	std::vector<std::pair<int32_t, int32_t>> directions{ {-1 , -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1} };

	for (auto [rowStep, colStep] : directions)
	{
		uint32_t steps(1);
		int32_t currentRow(int32_t(row) + rowStep), currentCol(int32_t(col) + colStep);
		while (steps <= maxSteps && currentRow >= 0 && currentRow < int32_t(grid.size()) && currentCol >= 0 && currentCol < int32_t(grid[0].length()))
		{
			if (grid[currentRow][currentCol] == FREE)
				break;
			else if (grid[currentRow][currentCol] == OCCUPIED)
			{
				++count;
				break;
			}

			currentRow += rowStep;
			currentCol += colStep;
			steps++;
		}
	}
	return count;
}

std::ostream& operator<<(std::ostream& os, const grid_t& grid)
{
	for (size_t i = 0; i < grid.size(); ++i)
	{
		os << grid[i];
		if (i < grid.size() - 1)
			os << std::endl;
	}
	return os;
}

grid_t execute(const grid_t& grid, NeighbourhoodType type, uint32_t occupiedLimit)
{
	grid_t newGrid(std::begin(grid), std::end(grid));
	bool changed = true;
	//size_t iteration(0);
	while (changed)
	{
		//std::cout << iteration++ << std::endl;
		grid_t oldGrid(begin(newGrid), end(newGrid));
		changed = false;
		for (size_t row = 0; row < grid.size(); ++row)
		{
			for (size_t col = 0; col < grid[0].length(); ++col)
			{
				switch (oldGrid[row][col])
				{
				case FREE:
					if (countOccupiedInNeighbourhood(oldGrid, row, col, type) == 0)
					{
						newGrid[row][col] = OCCUPIED;
						changed = true;
					}
					break;
				case OCCUPIED:
					if (countOccupiedInNeighbourhood(oldGrid, row, col, type) >= occupiedLimit)
					{
						newGrid[row][col] = FREE;
						changed = true;
					}
					break;
				}
			}
		}
		//std::cout << newGrid << std::endl << std::endl;
		//std::string line;
		//std::getline(std::cin, line);
	}
	return newGrid;
}

int main()
{
	using std::begin;
	using std::end;

	//std::istringstream is(EXAMPLE);
	std::ifstream is("input.txt");
	grid_t grid;

	std::string line;
	while (std::getline(is, line) && !line.empty())
		grid.push_back(line);
	
	int32_t part1Count(0); 
	auto grid1 = execute(grid, NeighbourhoodType::Nearest, 4);
	for (auto& row : grid1)
		part1Count += int32_t(std::count(begin(row), end(row), '#'));
	std::cout << "Day11 Part 1: " << part1Count << std::endl;

	int32_t part2Count(0); 
	auto grid2 = execute(grid, NeighbourhoodType::LineOfSight, 5);
	for (auto& row : grid2)
		part2Count += int32_t(std::count(begin(row), end(row), '#'));
	std::cout << "Day11 Part 2: " << part2Count << std::endl;

}
