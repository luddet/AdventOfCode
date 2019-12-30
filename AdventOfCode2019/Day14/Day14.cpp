// https://adventofcode.com/2019/day/14
//

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <tuple>
#include <algorithm>

const char* TESTINPUT1 =
"10 ORE => 10 A\n"
"1 ORE => 1 B\n"
"7 A, 1 B => 1 C\n"
"7 A, 1 C => 1 D\n"
"7 A, 1 D => 1 E\n"
"7 A, 1 E => 1 FUEL";

class Reaction
{
public:
	Reaction(const std::string output, uint8_t outputCount, const std::vector<std::tuple<std::string, uint8_t>>& inputs)
		: m_output(output),
		m_outputCount(outputCount),
		m_inputs(inputs)
	{}

	const std::string& getOutput() const;
	const uint8_t getOutputCount() const;
	const std::vector<std::tuple<std::string, uint8_t>>& getInputs() const;

private:
	std::string m_output;
	uint8_t m_outputCount;
	std::vector<std::tuple<std::string, uint8_t>> m_inputs;
};

const std::string& Reaction::getOutput() const
{
	return m_output;
}

const uint8_t Reaction::getOutputCount() const
{
	return m_outputCount;
}

const std::vector<std::tuple<std::string, uint8_t>>& Reaction::getInputs() const
{
	return m_inputs;
}

Reaction parseReaction(const std::string& reactionInput)
{
	std::istringstream is(reactionInput);


	std::vector<std::tuple<std::string, uint8_t>> inputs;
	std::string id, delim;
	int count;

	size_t offset = 0;
	std::string delimiters(" ,=>");

	auto segmentStart = std::begin(reactionInput) + offset;
	auto segmentEndIt = std::find_first_of(segmentStart, std::end(reactionInput), std::begin(delimiters), std::end(delimiters));

	count = std::stoi(std::string(segmentStart, segmentEndIt));

	




	return Reaction(id, count, inputs);
}

int main()
{
	std::istringstream is(TESTINPUT1);
	std::string line;
	std::vector<Reaction> reactions;
	while (std::getline(is, line))
		reactions.emplace_back(parseReaction(line));

}
