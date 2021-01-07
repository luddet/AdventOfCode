#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cassert>
#include <numeric>
#include <stack>
#include <map>
#include <bitset>

const char* EXAMPLE[] = {
"mask = XXXXXXXXXXXXXXXXXXXXXXXXXXXXX1XXXX0X\n"
"mem[8] = 11\n"
"mem[7] = 101\n"
"mem[8] = 0",
"mask = 000000000000000000000000000000X1001X\n"
"mem[42] = 100\n"
"mask = 00000000000000000000000000000000X0XX\n"
"mem[26] = 1" };

const std::string MASK_PREFIX("mask = ");

class Mask
{
public:
	Mask() = default;
	Mask(uint64_t orMask, uint64_t andMask, const std::string& maskString)
		:m_orMask(orMask), m_andMask(andMask), m_maskString(maskString)	{}

	uint64_t getOrMask() const { return m_orMask; }
	uint64_t getAndMask() const { return m_andMask; }
	const std::string& getMaskString() const { return m_maskString; }
private:
	uint64_t m_orMask = { 0 };
	uint64_t m_andMask = { UINT64_MAX };
	std::string m_maskString = { "" };
};

std::ostream& operator<<(std::ostream& os, const Mask& mask)
{
	os << "str: ";
	os.width(64);
	os << mask.getMaskString() << std::endl;
	os << " or: " << std::bitset<64>{mask.getOrMask()} << std::endl;
	os << "and: " << std::bitset<64>{mask.getAndMask()};
	return os;
}

Mask parseMask(const std::string& line)
{
	uint64_t orMask = 0;
	uint64_t andMask = UINT64_MAX;

	assert(line.find(MASK_PREFIX) == 0);
	std::string maskString = line.substr(MASK_PREFIX.length());
	for (size_t i = 0; i < maskString.length(); ++i)
	{
		switch (maskString[i])
		{
			case '1':
				orMask |= 1ull << (maskString.length() - i - 1);
				break;
			case '0':
				andMask &= ~(1ull << (maskString.length() - i - 1));
				break;
		}		
	}
	return Mask{ orMask, andMask, maskString };
}

std::pair<uint64_t, uint64_t> parseMem(const std::string& line)
{
	auto adressStart = line.find('[') + 1;
	uint64_t adress = std::stoi(line.substr(adressStart, line.find(']') - adressStart));
	uint64_t value = std::stoi(line.substr(line.find("= ") + 2));
	return { adress, value };
}

enum class InstructionType
{
	Mask,
	Mem
};

class Instruction
{
public:
	Instruction(const std::string& line)
	{
		if (line.find(MASK_PREFIX) == 0)
		{
			m_type = InstructionType::Mask;
			m_mask = parseMask(line);
		}
		else
		{
			m_type = InstructionType::Mem;
			m_mem = parseMem(line);
		}
	}

	Instruction(const Mask& mask) : m_type(InstructionType::Mask), m_mask(mask) {}
	Instruction(std::pair<uint64_t, uint64_t> mem) : m_type(InstructionType::Mem), m_mem(mem){}

	InstructionType getType() const { return m_type; }
	const Mask& getMask() const { return m_mask; }
	const std::pair<uint64_t, uint64_t>& getMem() const { return m_mem; }

private:
	InstructionType m_type = {};
	Mask m_mask = {};
	std::pair<uint64_t, uint64_t> m_mem = {};
};

uint64_t executePart1(const std::vector<Instruction>& instructions)
{
	Mask mask;
	std::vector<uint64_t> memory;
	for (auto& instruction : instructions)
	{
		if (instruction.getType() == InstructionType::Mask)
			mask = instruction.getMask();
		else if (instruction.getType() == InstructionType::Mem)
		{
			auto [adress, value] = instruction.getMem();
			value &= mask.getAndMask();
			value |= mask.getOrMask();

			if (memory.size() <= adress)
				memory.resize(adress + 1ull);
			memory[adress] = value;
		}
	}

	return std::accumulate(std::begin(memory), std::end(memory), 0ull);
}

uint64_t executePart2(const std::vector<Instruction>& instructions)
{
	Mask mask;
	std::map<uint64_t, uint64_t> memory;
	for (auto& instruction : instructions)
	{
		if (instruction.getType() == InstructionType::Mask)
		{
			mask = instruction.getMask();
			//std::cout << mask << std::endl;
		}
		else if (instruction.getType() == InstructionType::Mem)
		{
			auto [orgAdress, value] = instruction.getMem();

			// <adress, depth>
			std::stack<std::pair<uint64_t, uint32_t>> adresses;
			adresses.push({ orgAdress, 0 });

			while (!adresses.empty())
			{
				auto [adress, depth] = adresses.top();
				adresses.pop();

				auto& maskStr = mask.getMaskString();
				while (depth < maskStr.length() && maskStr[depth] != 'X')
					++depth;

				if (depth < maskStr.length())
				{
					auto shiftedMask = 1ull << (maskStr.length() - depth - 1);
					adress |= shiftedMask;
					adresses.push({ adress, depth + 1 });
					adress &= ~shiftedMask;
					adresses.push({ adress, depth + 1 });
				}
				else
				{
					adress |= mask.getOrMask();
					memory[adress] = value;
//					std::cout << std::bitset<64>{adress} << std::endl;
				}
			}
		}
		//std::cout << std::endl;
	}

	uint64_t sum = 0;
	for (auto& p : memory)
		sum += p.second;
	return sum;
}


int main()
{
	//std::istringstream is(EXAMPLE[1]);
	std::ifstream is("input.txt");

	std::vector<Instruction> instructions;

	std::string line;
	while (std::getline(is, line) && !std::empty(line))
		instructions.push_back({ line });

	auto part1 = executePart1(instructions);
	auto part2 = executePart2(instructions);

	std::cout << "Day14 Part 1: " << part1 << std::endl;
	std::cout << "Day14 Part 2: " << part2 << std::endl;
}
