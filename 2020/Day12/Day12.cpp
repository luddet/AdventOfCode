#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <array>
#include <vector>

std::string EXAMPLE("F10\nN3\nF7\nR90\nF11\n");

using instructions_t = std::vector<std::pair<char, int32_t>>;

instructions_t parseInput(std::istream& is)
{
	instructions_t instructions;
	std::string line;
	while (std::getline(is, line) && !std::empty(line))
	{
		char instruction = line[0];
		int32_t param = std::stoi(line.substr(1));
		instructions.push_back({ instruction, param });
	}
	return instructions;
}

void move(char dir, int32_t distance, int32_t& posX, int32_t& posY)
{
	switch (dir)
	{
	case 'N':
		posY += distance;
		break;
	case 'E':
		posX += distance;
		break;
	case 'S':
		posY -= distance;
		break;
	case 'W':
		posX -= distance;
		break;
	}
}

int32_t executePart1(const instructions_t& instructions)
{
	const std::array<char, 4> directions{ 'N', 'E', 'S', 'W' };
	int32_t posX(0), posY(0);
	uint8_t dir(1);

	for(auto [instruction, param] : instructions)
	{
		switch (instruction)
		{
		case 'N':
		case 'E':
		case 'S':
		case 'W':
			move(instruction, param, posX, posY);
			break;
		case 'F':
			move(directions[dir], param, posX, posY);
			break;
		case 'L':
			dir = (directions.size() + int(dir) - (param / 90)) % 4;
			break;
		case 'R':
			dir = (dir + (param / 90)) % 4;
			break;
		}
	}

	return std::abs(posX) + std::abs(posY);
}

struct Pos
{
	int32_t x;
	int32_t y;
};

// dir: either 'L' or 'R'
// angle: even multiple of 90
void simpleRotate(const char dir, int32_t angle, Pos& point)
{
	int32_t iterations = angle / 90;
	for (size_t i = 0; i < iterations; ++i)
	{
		if (dir == 'R')
			point.x = -point.x;
		else
			point.y = -point.y;
		std::swap(point.x, point.y);
	}
}

int32_t executePart2(const instructions_t& instructions)
{
	Pos ship { 0,0 };
	Pos waypoint { 10, 1 };

	for (auto [i, p] : instructions)
	{
		switch (i)
		{
			case 'N':
				waypoint.y += p;
				break;
			case 'E':
				waypoint.x += p;
				break;
			case 'S':
				waypoint.y -= p;
				break;
			case 'W':
				waypoint.x -= p;
				break;
			case 'L':
			case 'R':
				simpleRotate(i, p, waypoint);
				break;
			case 'F':
				ship.x += p * waypoint.x;
				ship.y += p * waypoint.y;
				break;
		}
	}
	return std::abs(ship.x) + std::abs(ship.y);
}

int main()
{
	//std::istringstream is(EXAMPLE);
	std::ifstream is("input.txt");

	instructions_t instructions(parseInput(is));

	auto part1 = executePart1(instructions);
	auto part2 = executePart2(instructions);

	
	std::cout << "Day12 part 1: " << part1 << std::endl;
	std::cout << "Day12 part 2: " << part2 << std::endl;
}
