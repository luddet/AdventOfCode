// https://adventofcode.com/2019/day/11

#include <iostream>
#include <fstream>
#include <functional>
#include <map>
#include <array>
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

Orientation rotate(Orientation orientation, Orientation direction)
{
	assert(direction == Orientation::Left || direction == Orientation::Right);

	int add = direction == Orientation::Right ? 1 : (int)Orientation::OrientationCount - 1;
	return (Orientation)(((int)orientation + add) % (int)Orientation::OrientationCount);
}


using panel_container_t = std::map<coord_t, Color>;

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

	Computer computer(originalMemory);
	panel_container_t panels;
	coord_t robotPosition{0, 0};
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

	std::cout << "Part 1: " << panels.size() << std::endl;
}

