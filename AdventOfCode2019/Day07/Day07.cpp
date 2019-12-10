// https://adventofcode.com/2019/day/7

#include <iostream>
#include <streambuf>
#include <fstream>
#include <sstream>
#include <vector>
#include <array>


template<size_t SIZE>
std::ostream& operator<<(std::ostream& os, const std::array<uint8_t, SIZE>& arr)
{
	for (size_t i = 0; i < SIZE - 1; ++i)
		os << (int)arr[i] << ", ";
	os << (int)arr[SIZE - 1];
	return os;
}

// Simple naive permutation generation, good enough for five elements
template<size_t SIZE>
std::vector<std::array<uint8_t, SIZE>> getPermutations(const std::array<uint8_t, SIZE>& sequence)
{
	std::vector<std::array<uint8_t, SIZE>> permutations;
	for (size_t i = 0; i < SIZE; ++i)
	{
		std::array<uint8_t, SIZE - 1> subSequence;
		for (size_t subIndex = 0, orgIndex = 0; subIndex < SIZE-1; ++subIndex, ++orgIndex)
		{
			if (i == orgIndex)
				++orgIndex;
			subSequence[subIndex] = sequence[orgIndex];
		}

		auto subPerms = getPermutations(subSequence);

		for (auto sub : subPerms)
		{
			std::array<uint8_t, SIZE> currentPerm;
			currentPerm[0] = sequence[i];
			for (size_t subIndex = 1; subIndex < SIZE; ++subIndex)
				currentPerm[subIndex] = sub[subIndex - 1];
			permutations.push_back(currentPerm);
		}
	}
	return permutations;
}

template<>
std::vector<std::array<uint8_t, 2>> getPermutations(const std::array<uint8_t, 2>& sequence)
{
	std::vector<std::array<uint8_t, 2>> permutations;
	permutations.push_back(std::array<uint8_t, 2>{sequence});
	permutations.push_back(std::array<uint8_t, 2>{sequence[1], sequence[0]});
	return permutations;
}

int runProgram(std::vector<int>& program, const std::vector<int>& input)
{
	size_t inputIndex = 0;
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
			program[program[ip + 1]] = input[inputIndex++];
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
	constexpr uint8_t PHASE_COUNT = 5;
	std::array<uint8_t, PHASE_COUNT> phases{ 0, 1, 2, 3, 4 };

	auto permutations = getPermutations(phases);

	std::ifstream fs("input.txt");
	std::vector<int> originalMemory(readInput(fs));

	size_t permutationCount = 0;
	int maxOutput = INT_MIN;
	for (auto permutation : permutations)
	{
		//std::cout << "Permutation " << permutationCount++ << ": " << permutation << std::endl;
		std::vector<int> inputs(2);
		int output = 0;
		inputs[0] = 0;
		inputs[1] = output;
		for (size_t i = 0; i < permutation.size(); ++i)
		{
			std::vector<int> ampMemory(originalMemory);
			inputs[0] = permutation[i];
			inputs[1] = output;
			output = runProgram(ampMemory, inputs);
		}
		if (output > maxOutput)
			maxOutput = output;
	}
	std::cout << "Part 1: " << maxOutput << std::endl;

	

	
}

