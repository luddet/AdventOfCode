// https://adventofcode.com/2019/day/2
#include <iostream>
#include <fstream>
#include <vector>

void runProgram(std::vector<int>& program)
{
	size_t instructionStride = 4;
	int opCode = 0;
	for (size_t ip = 0; ip < program.size() && opCode != 99; ip += instructionStride)
	{
		opCode = program[ip];
		switch (opCode)
		{
		case 1:
			program[program[ip + 3]] = program[program[ip + 1]] + program[program[ip + 2]];
			break;
		case 2:
			program[program[ip + 3]] = program[program[ip + 1]] * program[program[ip + 2]];
			break;
		case 99:
			break;
		default:
			throw std::exception("Unhandled opcode", opCode);
		}
	}
}

int main()
{
	std::ifstream fs("input.txt");
	std::vector<int> originalMemory;
	int i;
	while (fs >> i)
	{
		originalMemory.push_back(i);
		char comma;
		fs >> comma; // eat comma
	}

	std::vector<int> part1Memory(originalMemory);
	part1Memory[1] = 12;
	part1Memory[2] = 2;
	runProgram(part1Memory);

	std::cout << "Part 1: " << part1Memory[0] << std::endl;

	bool done = false;
	size_t noun, verb;
	for (noun = 0; noun <= 99; ++noun)
	{
		for (verb = 0; verb <= 99; ++verb)
		{
			std::vector<int> part2Memory(originalMemory);
			part2Memory[1] = noun;
			part2Memory[2] = verb;
			runProgram(part2Memory);
			done = part2Memory[0] == 19690720;
			if (done)
				break;
		}
		if (done)
			break;
	}

	if (!done)
	{
		std::cout << "No solution found." << std::endl;
		return 1;
	}

	std::cout << "Part 2: " << (noun * 100 + verb) << std::endl;
}

