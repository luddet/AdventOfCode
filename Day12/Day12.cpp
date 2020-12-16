#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <array>

std::string EXAMPLE("F10\nN3\nF7\nR90\nF11\n");

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

int main()
{
	//std::istringstream is(EXAMPLE);
	std::ifstream is("input.txt");

	const std::array<char, 4> directions{ 'N', 'E', 'S', 'W' };
	int32_t posX(0), posY(0);
	uint8_t dir(1);

	std::string line;
	while (std::getline(is, line) && !std::empty(line))
	{
		char instruction = line[0];
		int32_t param = std::stoi(line.substr(1));

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

	auto part1 = std::abs(posX) + std::abs(posY);
	std::cout << "Day12 part 1: " << part1 << std::endl;
}
