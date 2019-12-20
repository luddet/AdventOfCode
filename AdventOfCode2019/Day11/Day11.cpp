// https://adventofcode.com/2019/day/11

#include <iostream>
#include <fstream>
#include <functional>
#include <map>
#include <array>
#include <vector>
#include <cassert>
#include "Computer.h"

struct coord_t
{
	int x;
	int y;
};
coord_t& operator+=(coord_t& lhs, const coord_t& rhs)
{
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	return lhs;
}

bool operator<(const coord_t& lhs, const coord_t& rhs)
{
	return (lhs.x < rhs.x) ? true : (lhs.x == rhs.x && lhs.y < rhs.y);
}

enum class Color : uint8_t
{
	Black = 0,
	White
};

enum class Orientation : uint8_t
{
	Up,
	Right,
	Down,
	Left,
	OrientationCount
};

using panel_container_t = std::map<coord_t, Color>;

Orientation rotate(Orientation orientation, Orientation direction)
{
	assert(direction == Orientation::Left || direction == Orientation::Right);

	int add = direction == Orientation::Right ? 1 : (int)Orientation::OrientationCount - 1;
	return (Orientation)(((int)orientation + add) % (int)Orientation::OrientationCount);
}

std::vector<std::vector<Color>> createGrid(const panel_container_t& panels)
{
	// Find bounds
	coord_t bottomLeft{ INT_MAX, INT_MAX };
	coord_t topRight{ INT_MIN, INT_MIN };
	for (auto& panel : panels)
	{
		if (panel.second == Color::Black)
			continue;

		coord_t c = panel.first;
		if (c.x < bottomLeft.x)
			bottomLeft.x = c.x;
		if (c.y < bottomLeft.y)
			bottomLeft.y = c.y;
		if (c.x > topRight.x)
			topRight.x = c.x;
		if (c.y > topRight.y)
			topRight.y = c.y;
	}

	// fill grid
	int gridWidth = topRight.x - bottomLeft.x + 1;
	int gridHeight = topRight.y - bottomLeft.y + 1;
	std::vector<std::vector<Color>> grid;
	for (size_t i = 0; i < gridHeight; ++i)
		grid.push_back(std::vector<Color>(gridWidth, Color::Black));

	for (auto& panel : panels)
	{
		if (panel.first.x >= bottomLeft.x && panel.first.y >= bottomLeft.y && panel.first.x <= topRight.x && panel.first.y <= topRight.y)
			grid[(grid.size()-1) - ((long)panel.first.y - bottomLeft.y)][(long)panel.first.x - bottomLeft.x] = panel.second;
	}

	return grid;
}

std::ostream& operator<<(std::ostream& os, const std::vector<std::vector<Color>>& grid)
{
	for (size_t row = 0; row < grid.size(); ++row)
	{
		for (size_t col = 0; col < grid[row].size(); ++col)
		{
			os << (grid[row][col] == Color::White ? "#" : " ");
		}
		os << std::endl;
	}
	return os;
}

int runRobot(const Computer::mem_container_t& startingMemory, panel_container_t& panels)
{
	Computer computer(startingMemory);
	coord_t robotPosition{ 0, 0 };
	Orientation robotOrientation = Orientation::Up;

	// indexed by Orientation
	std::array<coord_t, 4> movementVectors{ 0, 1, 1, 0, 0, -1, -1, 0 };

	while (!computer.isDone())
	{
		// get color of current position
		auto currentPanelIt = panels.find(robotPosition);
		Color currentColor;
		if (currentPanelIt == panels.end())
			currentColor = Color::Black;
		else
			currentColor = (*currentPanelIt).second;

		// feed it to computer
		computer.addInput((int)currentColor);

		// run computer
		computer.run();

		// read paint output
		Computer::mem_item_t output;
		computer.getOutput(output);

		// paint panel
		if (currentPanelIt == panels.end())
			panels.insert(std::make_pair(robotPosition, (Color)output));
		else
			(*currentPanelIt).second = (Color)output;

		// read turning output
		computer.getOutput(output);

		// turn
		robotOrientation = rotate(robotOrientation, output == 0 ? Orientation::Left : Orientation::Right);

		// move robot
		robotPosition += movementVectors[(size_t)robotOrientation];
	}
	return panels.size();
}

int main()
{
	std::fstream fs("input.txt");
	Computer::mem_container_t originalMemory;
	Computer::mem_item_t opCode;
	while (fs >> opCode)
	{
		originalMemory.push_back(opCode);
		char c; fs >> c; // eat comma
	}

	panel_container_t part1Panels;
	int numPainted = runRobot(originalMemory, part1Panels);

	std::cout << "Part 1: " << numPainted << std::endl;

	panel_container_t part2Panels;
	part2Panels.insert(std::make_pair(coord_t{ 0,0 }, Color::White));
	runRobot(originalMemory, part2Panels);
	auto grid = createGrid(part2Panels);

	std::cout << "Part 2:" << std::endl;
	std::cout << grid << std::endl;
	// JHARBGCU
}

