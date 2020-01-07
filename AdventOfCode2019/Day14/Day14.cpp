// https://adventofcode.com/2019/day/14
//

#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <tuple>
#include <algorithm>
#include <stack>
#include <unordered_map>

uint32_t TEST_1_ORE_REQ = 31;
const char* TEST_1_INPUT =
"10 ORE => 10 A\n"
"1 ORE => 1 B\n"
"7 A, 1 B => 1 C\n"
"7 A, 1 C => 1 D\n"
"7 A, 1 D => 1 E\n"
"7 A, 1 E => 1 FUEL";

uint32_t TEST_2_ORE_REQ = 165;
const char* TEST_2_INPUT =
"9 ORE => 2 A\n"
"8 ORE => 3 B\n"
"7 ORE => 5 C\n"
"3 A, 4 B => 1 AB\n"
"5 B, 7 C => 1 BC\n"
"4 C, 1 A => 1 CA\n"
"2 AB, 3 BC, 4 CA => 1 FUEL";

uint32_t TEST_3_ORE_REQ = 13312;
const char* TEST_3_INPUT =
"157 ORE => 5 NZVS\n"
"165 ORE => 6 DCFZ\n"
"44 XJWVT, 5 KHKGT, 1 QDVJ, 29 NZVS, 9 GPVTF, 48 HKGWZ => 1 FUEL\n"
"12 HKGWZ, 1 GPVTF, 8 PSHF => 9 QDVJ\n"
"179 ORE => 7 PSHF\n"
"177 ORE => 5 HKGWZ\n"
"7 DCFZ, 7 PSHF => 2 XJWVT\n"
"165 ORE => 2 GPVTF\n"
"3 DCFZ, 7 NZVS, 5 HKGWZ, 10 PSHF => 8 KHKGT";

uint32_t TEST_4_ORE_REQ = 180697;
const char* TEST_4_INPUT =
"2 VPVL, 7 FWMGM, 2 CXFTF, 11 MNCFX => 1 STKFG\n"
"17 NVRVD, 3 JNWZP => 8 VPVL\n"
"53 STKFG, 6 MNCFX, 46 VJHF, 81 HVMC, 68 CXFTF, 25 GNMV => 1 FUEL\n"
"22 VJHF, 37 MNCFX => 5 FWMGM\n"
"139 ORE => 4 NVRVD\n"
"144 ORE => 7 JNWZP\n"
"5 MNCFX, 7 RFSQX, 2 FWMGM, 2 VPVL, 19 CXFTF => 3 HVMC\n"
"5 VJHF, 7 MNCFX, 9 VPVL, 37 CXFTF => 6 GNMV\n"
"145 ORE => 6 MNCFX\n"
"1 NVRVD => 8 CXFTF\n"
"1 VJHF, 6 MNCFX => 4 RFSQX\n"
"176 ORE => 6 VJHF";

uint32_t TEST_5_ORE_REQ = 2210736;
const char* TEST_5_INPUT =
"171 ORE => 8 CNZTR\n"
"7 ZLQW, 3 BMBT, 9 XCVML, 26 XMNCP, 1 WPTQ, 2 MZWV, 1 RJRHP => 4 PLWSL\n"
"114 ORE => 4 BHXH\n"
"14 VRPVC => 6 BMBT\n"
"6 BHXH, 18 KTJDG, 12 WPTQ, 7 PLWSL, 31 FHTLT, 37 ZDVW => 1 FUEL\n"
"6 WPTQ, 2 BMBT, 8 ZLQW, 18 KTJDG, 1 XMNCP, 6 MZWV, 1 RJRHP => 6 FHTLT\n"
"15 XDBXC, 2 LTCX, 1 VRPVC => 6 ZLQW\n"
"13 WPTQ, 10 LTCX, 3 RJRHP, 14 XMNCP, 2 MZWV, 1 ZLQW => 1 ZDVW\n"
"5 BMBT => 4 WPTQ\n"
"189 ORE => 9 KTJDG\n"
"1 MZWV, 17 XDBXC, 3 XCVML => 2 XMNCP\n"
"12 VRPVC, 27 CNZTR => 2 XDBXC\n"
"15 KTJDG, 12 BHXH => 5 XCVML\n"
"3 BHXH, 2 VRPVC => 7 MZWV\n"
"121 ORE => 7 VRPVC\n"
"7 XCVML => 6 RJRHP\n"
"5 BHXH, 4 VRPVC => 5 LTCX";

std::vector<std::tuple<const char*, uint32_t>> TEST_DATA{
	{TEST_1_INPUT, TEST_1_ORE_REQ},
	{TEST_2_INPUT, TEST_2_ORE_REQ},
	{TEST_3_INPUT, TEST_3_ORE_REQ},
	{TEST_4_INPUT, TEST_4_ORE_REQ},
	{TEST_5_INPUT, TEST_5_ORE_REQ}
};

class Reaction
{
public:
	using input_t = std::tuple<std::string, uint32_t>;
	using input_container_t = std::vector<input_t>;

	Reaction(const std::string output, uint32_t outputCount, const input_container_t& inputs)
		: m_output(output),
		m_outputCount(outputCount),
		m_inputs(inputs)
	{}

	const std::string& getOutput() const;
	const uint32_t getOutputCount() const;
	const input_container_t& getInputs() const;

private:
	std::string m_output;
	uint32_t m_outputCount;
	input_container_t m_inputs;
};

const std::string&
Reaction::getOutput() const
{
	return m_output;
}

const uint32_t
Reaction::getOutputCount() const
{
	return m_outputCount;
}

const Reaction::input_container_t&
Reaction::getInputs() const
{
	return m_inputs;
}

Reaction
parseReaction(const std::string& reactionInput)
{
	const std::string delimiters(" ,=>");
	Reaction::input_container_t inputs;
	std::string id;
	int32_t count;

	std::string::const_iterator segmentStartIt = std::begin(reactionInput);
	std::string::const_iterator segmentEndIt;

	do
	{
		segmentEndIt = std::find_first_of(segmentStartIt, std::end(reactionInput), std::begin(delimiters), std::end(delimiters));
		count = std::stoi(std::string(segmentStartIt, segmentEndIt));

		segmentStartIt = std::find_if(segmentEndIt, std::end(reactionInput), [](char c) {return std::isalpha(c); });
		segmentEndIt = std::find_first_of(segmentStartIt, std::end(reactionInput), std::begin(delimiters), std::end(delimiters));
		id = std::string(segmentStartIt, segmentEndIt);

		inputs.push_back(std::make_tuple(id, count));

		while (*segmentEndIt == ' ')
			++segmentEndIt;

		segmentStartIt = std::find_if_not(segmentEndIt, std::end(reactionInput), [&](char c) { return delimiters.find(c) != std::string::npos; });
	} while (*segmentEndIt == ',');

	segmentEndIt = std::find_first_of(segmentStartIt, std::end(reactionInput), std::begin(delimiters), std::end(delimiters));
	count = std::stoi(std::string(segmentStartIt, segmentEndIt));

	segmentStartIt = std::find_if(segmentEndIt, std::end(reactionInput), [](char c) {return std::isalpha(c); });
	segmentEndIt = std::find_first_of(segmentStartIt, std::end(reactionInput), std::begin(delimiters), std::end(delimiters));
	id = std::string(segmentStartIt, segmentEndIt);

	return Reaction(id, count, inputs);
}




int main()
{
	std::fstream fs("input.txt");
	std::string line;
	std::vector<Reaction> reactions;
	while (std::getline(fs, line))
		reactions.emplace_back(parseReaction(line));

	std::stack<std::tuple<std::string, uint32_t>> productionStack;
	std::unordered_map<std::string, uint32_t> chemicalStore;

	productionStack.push(std::make_tuple("FUEL", 1));
	
	uint32_t consumedOre = 0;
	while (!productionStack.empty())
	{
		// get top
		const std::string& nameToProduce = std::get<0>(productionStack.top());
		// find reaction
		Reaction& reaction = *std::find_if(std::begin(reactions), std::end(reactions), [&nameToProduce](const Reaction& r) { return r.getOutput() == nameToProduce; });
		uint32_t batchSize = reaction.getOutputCount();

		uint32_t batchesToProduce;
		auto inStoreIt = chemicalStore.find(nameToProduce);
		uint32_t amountInStore = inStoreIt == chemicalStore.end() ? 0 : (*inStoreIt).second;
		if (amountInStore >= std::get<1>(productionStack.top())) // already enough in store
		{
			productionStack.pop();
			continue;
		}

		uint32_t amountNeeded = std::get<1>(productionStack.top()) - amountInStore;
		
		batchesToProduce = ( (amountNeeded / batchSize) + (amountNeeded % batchSize == 0 ? 0 : 1) );

		// check store for needed chems for top
		std::vector<std::tuple<std::string, uint32_t>> missingChems;
		std::for_each(std::begin(reaction.getInputs()), std::end(reaction.getInputs()), [&chemicalStore, &missingChems, batchesToProduce](Reaction::input_t input)
			{
				std::string inputName = std::get<0>(input);
				if (inputName == "ORE") // WE have infinite ORE
					return;
				std::uint32_t inputAmountRequired = std::get<1>(input) * batchesToProduce;

				auto storeIt = chemicalStore.find(inputName);
				uint32_t amountInStore = storeIt == chemicalStore.end() ? 0 : (*storeIt).second;
				if (amountInStore < inputAmountRequired)
					missingChems.push_back(std::make_tuple(inputName, inputAmountRequired));
			});

		// if store contains enough
		if (missingChems.empty())
		{
			// reduce store
			std::for_each(std::begin(reaction.getInputs()), std::end(reaction.getInputs()), [&chemicalStore, &consumedOre, batchesToProduce](Reaction::input_t input)
				{
					std::string inputName = std::get<0>(input);
					std::uint32_t inputAmountRequired = std::get<1>(input) * batchesToProduce;

					if (inputName == "ORE")
					{
						consumedOre += inputAmountRequired;
					}
					else
					{
						auto storeIt = chemicalStore.find(inputName);
						(*storeIt).second -= inputAmountRequired;
					}
				});
			// and add output to store
			auto storeIt = chemicalStore.find(nameToProduce);
			if (storeIt == chemicalStore.end())
			{
				chemicalStore.insert(std::make_pair(nameToProduce, batchesToProduce * batchSize));
			}
			else
			{
				(*storeIt).second += batchesToProduce * batchSize;
			}

			productionStack.pop();
		}
		else // else for each insufficient chem
		{
			// find chem reaction and push on productionstack
			std::for_each(missingChems.begin(), missingChems.end(), [&productionStack](auto t)
				{
					productionStack.push(t);
				});
		}

	}
	
	std::cout << "Part 1: " << consumedOre << std::endl;

}
