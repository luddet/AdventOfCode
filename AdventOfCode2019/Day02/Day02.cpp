// https://adventofcode.com/2019/day/2
#include <iostream>
#include <fstream>
#include <vector>
//#include <exception>

int main()
{
	std::ifstream fs("input.txt");
	std::vector<int> program;
	int i;
	while (fs >> i)
	{
		program.push_back(i);
		char comma;
		fs >> comma; // eat comma
	}

	// patch program
	program[1] = 12;
	program[2] = 2;

	size_t instructionStride = 4;
	int opCode = 0;
	for (size_t pc = 0; pc < program.size() && opCode != 99; pc += instructionStride)
	{
		opCode = program[pc];
		switch (opCode)
		{
			case 1:
				program[program[pc + 3]] = program[program[pc + 1]] + program[program[pc + 2]];
				break;
			case 2:
				program[program[pc + 3]] = program[program[pc + 1]] * program[program[pc + 2]];
				break;
			case 99:
				break;
			default:
				throw std::exception("Unhandled opcode", opCode);
		}
	}

	std::cout << "Part 1: " << program[0] << std::endl;
}

