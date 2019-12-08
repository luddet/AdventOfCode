// https://adventofcode.com/2019/day/5

#include <iostream>
#include <streambuf>
#include <fstream>
#include <sstream>
#include <vector>

int runProgram(std::vector<int>& program, int input)
{
	int output = -1;
	int opCode = 0;
	for (size_t ip = 0; ip < program.size() && opCode != 99; )
	{
		int mode[3]{ 0 };
		opCode = program[ip] % 100;
		mode[2] = (program[ip] / 100) % 10;
		mode[1] = (program[ip] / 1000) % 10;

		int param1Value, param2Value;
		switch (opCode)
		{
		case 1: // ADD
			param1Value = mode[2] == 0 ? program[program[ip + 1]] : program[ip + 1];
			param2Value = mode[1] == 0 ? program[program[ip + 2]] : program[ip + 2];
			program[program[ip + 3]] = param1Value + param2Value;
			ip += 4;
			break;
		case 2: // MUL
			param1Value = mode[2] == 0 ? program[program[ip + 1]] : program[ip + 1];
			param2Value = mode[1] == 0 ? program[program[ip + 2]] : program[ip + 2];
			program[program[ip + 3]] = param1Value * param2Value;
			ip += 4;
			break;
		case 3: // READ INPUT
			program[program[ip + 1]] = input;
			ip += 2;
			break;
		case 4: // OUTPUT
			param1Value = mode[2] == 0 ? program[program[ip + 1]] : program[ip + 1];
			output = param1Value;
//			std::cout << output << std::endl;
			ip += 2;
			break;
		case 5: // JUMP IF TRUE
			param1Value = mode[2] == 0 ? program[program[ip + 1]] : program[ip + 1];
			param2Value = mode[1] == 0 ? program[program[ip + 2]] : program[ip + 2];
			if (param1Value != 0)
				ip = param2Value;
			else
				ip += 3;
			break;
		case 6: // JUMP IF FALSE
			param1Value = mode[2] == 0 ? program[program[ip + 1]] : program[ip + 1];
			param2Value = mode[1] == 0 ? program[program[ip + 2]] : program[ip + 2];
			if (param1Value == 0)
				ip = param2Value;
			else
				ip += 3;
			break;
		case 7: // LESS THAN
			param1Value = mode[2] == 0 ? program[program[ip + 1]] : program[ip + 1];
			param2Value = mode[1] == 0 ? program[program[ip + 2]] : program[ip + 2];
			program[program[ip + 3]] = (param1Value < param2Value) ? 1 : 0;
			ip += 4;
			break;
		case 8: // EQUALS
			param1Value = mode[2] == 0 ? program[program[ip + 1]] : program[ip + 1];
			param2Value = mode[1] == 0 ? program[program[ip + 2]] : program[ip + 2];
			program[program[ip + 3]] = (param1Value == param2Value) ? 1 : 0;
			ip += 4;
			break;
		case 99:
			break;
		default:
			throw std::exception("Unhandled opcode", opCode);
		}
	}
	return output;
}

std::vector<int> readInput(std::istream& stream)
{
	std::vector<int> memory;
	int i;
	while (stream >> i)
	{
		memory.push_back(i);
		char comma;
		stream >> comma; // eat comma
	}
	return memory;
}

int main()
{
	std::ifstream fs("input.txt");
	std::vector<int> originalMemory(readInput(fs));

	std::vector<int> part1Memory(originalMemory);
	std::vector<int> part2Memory(originalMemory);
	
	int diagnosticsCode1 = runProgram(part1Memory, 1);
	int diagnosticsCode2 = runProgram(part2Memory, 5);

	std::cout << "Part 1: " << diagnosticsCode1 << std::endl;
	std::cout << "Part 2: " << diagnosticsCode2 << std::endl;

}

