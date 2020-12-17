#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cassert>
//#include <bitset>
#include <numeric>

const char* EXAMPLE =
"mask = XXXXXXXXXXXXXXXXXXXXXXXXXXXXX1XXXX0X\n"
"mem[8] = 11\n"
"mem[7] = 101\n"
"mem[8] = 0";

const std::string MASK_PREFIX("mask = ");

void parseMask(const std::string& line, uint64_t& orMask, uint64_t& andMask, std::string& maskString)
{
	orMask = 0;
	andMask = UINT64_MAX;

	assert(line.find(MASK_PREFIX) == 0);
	maskString = line.substr(MASK_PREFIX.length());
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
	//std::cout << "Mask:                             " << maskString << std::endl;
	//std::cout << "Mask: ";
	//std::cout.width(64);
	//std::cout << maskString << std::endl;
	//std::cout << "or  : " << std::bitset<64>(orMask) << std::endl;
	//std::cout << "and : " << std::bitset<64>(andMask) << std::endl;

}

void parseMem(const std::string& line, uint64_t& adress, uint64_t& value)
{
	auto adressStart = line.find('[') + 1;
	adress = std::stoi(line.substr(adressStart, line.find(']') - adressStart));
	value = std::stoi(line.substr(line.find("= ") + 2));
}

int main()
{
	//std::istringstream is(EXAMPLE);
	std::ifstream is("input.txt");

	uint64_t orMask(0);
	uint64_t andMask(UINT64_MAX);
	std::string maskString;

	std::vector<uint64_t> memory;

	std::string line;
	while (std::getline(is, line) && !std::empty(line))
	{
		if (line.find(MASK_PREFIX) == 0)
		{
			parseMask(line, orMask, andMask, maskString);
		}
		else if (line.find("mem") == 0)
		{
			uint64_t adress, value;
			parseMem(line, adress, value);

			value &= andMask;
			value |= orMask;

			if (memory.size() <= adress)
				memory.resize(adress + 1ull);
			memory[adress] = value;
		}
	}
	
	auto part1 = std::accumulate(std::begin(memory), std::end(memory), 0ull);
	std::cout << "Day14 Part 1: " << part1 << std::endl;
}
