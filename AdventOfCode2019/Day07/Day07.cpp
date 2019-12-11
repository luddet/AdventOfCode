// https://adventofcode.com/2019/day/7

#include <iostream>
#include <streambuf>
#include <fstream>
#include <sstream>
#include <vector>
#include <array>
#include <queue>

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

class Computer
{
public:
	enum class ComputerState
	{
		IDLE,
		WAITING_FOR_INPUT,
		DONE,
	};

	Computer(const std::vector<int>& memory)
		:m_ip(0),
		 m_state(ComputerState::IDLE)
	{
		std::copy(memory.cbegin(), memory.cend(), std::back_inserter(m_memory));
	}

	void addInput(int input)
	{
		m_input.push(input);
	}

	bool getOutput(int& output)
	{
		if (!m_output.empty())
		{
			output = m_output.front();
			m_output.pop();
			return true;
		}
		else
			return false;
	}

	bool isDone() const
	{
		return m_state == ComputerState::DONE;
	}

	void run()
	{
		int opCode = 0;
		while (m_ip < m_memory.size())
		{
			int mode[3]{ 0 };
			opCode = m_memory[m_ip] % 100;
			mode[2] = (m_memory[m_ip] / 100) % 10;
			mode[1] = (m_memory[m_ip] / 1000) % 10;

			int param1Value, param2Value;
			switch (opCode)
			{
			case 1: // ADD
				param1Value = mode[2] == 0 ? m_memory[m_memory[m_ip + 1]] : m_memory[m_ip + 1];
				param2Value = mode[1] == 0 ? m_memory[m_memory[m_ip + 2]] : m_memory[m_ip + 2];
				m_memory[m_memory[m_ip + 3]] = param1Value + param2Value;
				m_ip += 4;
				break;
			case 2: // MUL
				param1Value = mode[2] == 0 ? m_memory[m_memory[m_ip + 1]] : m_memory[m_ip + 1];
				param2Value = mode[1] == 0 ? m_memory[m_memory[m_ip + 2]] : m_memory[m_ip + 2];
				m_memory[m_memory[m_ip + 3]] = param1Value * param2Value;
				m_ip += 4;
				break;
			case 3: // READ INPUT
				if (m_input.empty())
				{
					m_state = ComputerState::WAITING_FOR_INPUT;
					return;
				}
				m_memory[m_memory[m_ip + 1]] = m_input.front();
				m_input.pop();
				m_ip += 2;
				break;
			case 4: // OUTPUT
				param1Value = mode[2] == 0 ? m_memory[m_memory[m_ip + 1]] : m_memory[m_ip + 1];
				m_output.push(param1Value);
				//			std::cout << output << std::endl;
				m_ip += 2;
				break;
			case 5: // JUMP IF TRUE
				param1Value = mode[2] == 0 ? m_memory[m_memory[m_ip + 1]] : m_memory[m_ip + 1];
				param2Value = mode[1] == 0 ? m_memory[m_memory[m_ip + 2]] : m_memory[m_ip + 2];
				if (param1Value != 0)
					m_ip = param2Value;
				else
					m_ip += 3;
				break;
			case 6: // JUMP IF FALSE
				param1Value = mode[2] == 0 ? m_memory[m_memory[m_ip + 1]] : m_memory[m_ip + 1];
				param2Value = mode[1] == 0 ? m_memory[m_memory[m_ip + 2]] : m_memory[m_ip + 2];
				if (param1Value == 0)
					m_ip = param2Value;
				else
					m_ip += 3;
				break;
			case 7: // LESS THAN
				param1Value = mode[2] == 0 ? m_memory[m_memory[m_ip + 1]] : m_memory[m_ip + 1];
				param2Value = mode[1] == 0 ? m_memory[m_memory[m_ip + 2]] : m_memory[m_ip + 2];
				m_memory[m_memory[m_ip + 3]] = (param1Value < param2Value) ? 1 : 0;
				m_ip += 4;
				break;
			case 8: // EQUALS
				param1Value = mode[2] == 0 ? m_memory[m_memory[m_ip + 1]] : m_memory[m_ip + 1];
				param2Value = mode[1] == 0 ? m_memory[m_memory[m_ip + 2]] : m_memory[m_ip + 2];
				m_memory[m_memory[m_ip + 3]] = (param1Value == param2Value) ? 1 : 0;
				m_ip += 4;
				break;
			case 99:
				m_state = ComputerState::DONE;
				return;
			default:
				throw std::exception("Unhandled opcode", opCode);
			}
		}
	}

private:
	size_t			 m_ip; // instruction pointer
	std::vector<int> m_memory;
	std::queue<int>	 m_input;
	std::queue<int>	 m_output;
	ComputerState	 m_state;
};

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

int part1(const std::vector<int>& originalMemory)
{
	constexpr uint8_t PHASE_COUNT = 5;
	std::array<uint8_t, PHASE_COUNT> phases{ 0, 1, 2, 3, 4 };

	auto permutations = getPermutations(phases);

	size_t permutationCount = 0;
	int maxOutput = INT_MIN;
	for (auto permutation : permutations)
	{
		int output = 0;
		for (size_t i = 0; i < permutation.size(); ++i)
		{
			Computer comp(originalMemory);
			comp.addInput(permutation[i]);
			comp.addInput(output);
			comp.run();
			comp.getOutput(output);
		}
		if (output > maxOutput)
			maxOutput = output;
	}
	return maxOutput;
}

int part2(const std::vector<int>& originalMemory)
{
	constexpr uint8_t PHASE_COUNT = 5;
	std::array<uint8_t, PHASE_COUNT> phases{ 5, 6, 7, 8, 9 };

	auto permutations = getPermutations(phases);

	int maxOutput = INT_MIN;
	for (auto permutation : permutations)
	{
		std::vector<Computer> amps;
		for (int i = 0; i < PHASE_COUNT; ++i)
			amps.push_back(Computer(originalMemory));

		// init with phase
		for (size_t i = 0; i < PHASE_COUNT; ++i)
		{
			amps[i].addInput(permutation[i]);
			amps[i].run();
		}

		int output = 0;
		bool allDone;
		do
		{
			allDone = true;
			for (size_t i = 0; i < PHASE_COUNT; ++i)
			{
				amps[i].addInput(output);
				amps[i].run();
				amps[i].getOutput(output);
				allDone &= amps[i].isDone();
			}
		} while (!allDone);

		maxOutput = std::max(output, maxOutput);
	}
	return maxOutput;
}

int main()
{
	std::ifstream fs("input.txt");
	std::vector<int> originalMemory(readInput(fs));

	std::cout << "Part 1: " << part1(originalMemory) << std::endl;
	std::cout << "Part 2: " << part2(originalMemory) << std::endl;
}

