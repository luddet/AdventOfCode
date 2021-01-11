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
#include <chrono>

uint64_t TEST_1_ORE_REQ = 31;
const char* TEST_1_INPUT =
"10 ORE => 10 A\n"
"1 ORE => 1 B\n"
"7 A, 1 B => 1 C\n"
"7 A, 1 C => 1 D\n"
"7 A, 1 D => 1 E\n"
"7 A, 1 E => 1 FUEL";

uint64_t TEST_2_ORE_REQ = 165;
const char* TEST_2_INPUT =
"9 ORE => 2 A\n"
"8 ORE => 3 B\n"
"7 ORE => 5 C\n"
"3 A, 4 B => 1 AB\n"
"5 B, 7 C => 1 BC\n"
"4 C, 1 A => 1 CA\n"
"2 AB, 3 BC, 4 CA => 1 FUEL";

uint64_t TEST_3_ORE_REQ = 13312;
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

uint64_t TEST_4_ORE_REQ = 180697;
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

uint64_t TEST_5_ORE_REQ = 2210736;
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

std::vector<std::tuple<const char*, uint64_t>> TEST_DATA{
	{TEST_1_INPUT, TEST_1_ORE_REQ},
	{TEST_2_INPUT, TEST_2_ORE_REQ},
	{TEST_3_INPUT, TEST_3_ORE_REQ},
	{TEST_4_INPUT, TEST_4_ORE_REQ},
	{TEST_5_INPUT, TEST_5_ORE_REQ}
};

class Reaction
{
public:
	using input_t = std::tuple<size_t, size_t>;
	using input_container_t = std::vector<input_t>;

	Reaction() = default;
	Reaction(size_t output, size_t outputCount, const input_container_t& inputs)
		: m_output(output),
		m_outputCount(outputCount),
		m_inputs(inputs)
	{}

	const size_t getOutput() const { return m_output; }
	const uint64_t getOutputCount() const { return m_outputCount; }
	const input_container_t& getInputs() const { return m_inputs; }

private:
	size_t m_output;
	uint64_t m_outputCount;
	input_container_t m_inputs;
};

using reactionIndexToNameMapping_t = std::unordered_map<std::string, size_t>;

size_t getIndex(const std::string& id, reactionIndexToNameMapping_t& mapping)
{
	auto mapIt = mapping.find(id);
	if (mapIt == mapping.end())
	{
		size_t nextIndex = mapping.empty() ? 0 :
			std::max_element(mapping.begin(), mapping.end(),
				[&](const auto& p1, const auto& p2) 
				{
					return p1.second < p2.second;
				})->second + 1;
		return mapping[id] = nextIndex;
	}
	else
		return mapIt->second;
}

Reaction
parseReaction(const std::string& reactionInput, reactionIndexToNameMapping_t& mapping)
{
	const std::string delimiters(" ,=>");
	Reaction::input_container_t inputs;
	std::string id;
	size_t index;
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

		index = getIndex(id, mapping);
		inputs.push_back({ index, count });

		while (*segmentEndIt == ' ')
			++segmentEndIt;

		segmentStartIt = std::find_if_not(segmentEndIt, std::end(reactionInput), [&](char c) { return delimiters.find(c) != std::string::npos; });
	} while (*segmentEndIt == ',');

	segmentEndIt = std::find_first_of(segmentStartIt, std::end(reactionInput), std::begin(delimiters), std::end(delimiters));
	count = std::stoi(std::string(segmentStartIt, segmentEndIt));

	segmentStartIt = std::find_if(segmentEndIt, std::end(reactionInput), [](char c) {return std::isalpha(c); });
	segmentEndIt = std::find_first_of(segmentStartIt, std::end(reactionInput), std::begin(delimiters), std::end(delimiters));
	id = std::string(segmentStartIt, segmentEndIt);
	index = getIndex(id, mapping);
	return Reaction(index, count, inputs);
}


std::vector<Reaction>
parseReactions(std::istream& stream, reactionIndexToNameMapping_t& mapping)
{
	std::string line;
	std::vector<Reaction> reactions;
	while (std::getline(stream, line))
	{
		auto reaction{ parseReaction(line, mapping) };
		auto index{ reaction.getOutput() };
		if (index >= reactions.size())
			reactions.resize(index + 1);
		reactions.at(index) = reaction;
	}
	return reactions;
}

using store_t = std::vector<size_t>;

template<class ReactionsContainer>
bool produce(size_t index, size_t amount, store_t& store, ReactionsContainer& reactions)
{
	std::stack<std::tuple<size_t, size_t>> productionStack;
	productionStack.push({ index, amount });
	while (!productionStack.empty())
	{
		// get top
		const auto& [indexToProduce, amountToProduce] = productionStack.top();
		const Reaction& reaction = reactions[indexToProduce];

		uint64_t amountInStore = store[indexToProduce];
		if (amountInStore >= amountToProduce) // already enough in store
		{
			productionStack.pop();
			continue;
		}

		uint64_t amountNeeded = amountToProduce - amountInStore;

		uint64_t batchSize = reaction.getOutputCount();
		if (batchSize == 0) // This reaction (ORE) yields nothing
			return false;

		uint64_t batchesToProduce = ((amountNeeded / batchSize) + (amountNeeded % batchSize == 0 ? 0 : 1));

		// check store for needed chems for top
		std::vector<std::tuple<size_t, uint64_t>> missingChems;
		std::for_each(std::begin(reaction.getInputs()), std::end(reaction.getInputs()), [&store, &missingChems, batchesToProduce](Reaction::input_t input)
			{
				size_t inputIndex = std::get<0>(input);
				uint64_t inputAmountRequired = std::get<1>(input) * batchesToProduce;

				uint64_t amountInStore = store[inputIndex];
				if (amountInStore < inputAmountRequired)
					missingChems.push_back({ inputIndex, inputAmountRequired });
			});

		// if store contains enough
		if (missingChems.empty())
		{
			// reduce store
			std::for_each(std::begin(reaction.getInputs()), std::end(reaction.getInputs()), [&store, batchesToProduce](Reaction::input_t input)
				{
					size_t inputIndex = std::get<0>(input);
					size_t inputAmountRequired = std::get<1>(input) * batchesToProduce;
					store[inputIndex] -= inputAmountRequired;
				});
			// and add output to store
			store[indexToProduce] += batchesToProduce * batchSize;

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
	return true;
}

bool runTests()
{
	return std::all_of(std::begin(TEST_DATA), std::end(TEST_DATA), [](std::tuple<const char*, uint64_t> testItem)
		{
			reactionIndexToNameMapping_t map{};
			std::istringstream is(std::get<0>(testItem));
			uint64_t expectedResult = std::get<1>(testItem);

			auto reactions = parseReactions(is, map);
			const size_t oreIndex = getIndex("ORE", map);
			const size_t fuelIndex = getIndex("FUEL", map);

			uint64_t initialOreAmount = 1000000000000;
			store_t chemicalStore( map.size(), 0 );
			chemicalStore[oreIndex] = initialOreAmount;

			produce(fuelIndex, 1, chemicalStore, reactions);

			auto oreConsumed = initialOreAmount - chemicalStore[oreIndex];

			return oreConsumed == expectedResult;
		});
}


int main()
{
	assert(runTests());

	//std::istringstream is(TEST_3_INPUT);

	std::fstream is("input.txt");

	reactionIndexToNameMapping_t nameMappings{};

	auto reactions = parseReactions(is, nameMappings);

	const size_t oreIndex = getIndex("ORE", nameMappings);
	const size_t fuelIndex = getIndex("FUEL", nameMappings);

	store_t chemicalStore( nameMappings.size(), 0);

	uint64_t initialOreAmount = 1000000000000;
	chemicalStore[oreIndex] = initialOreAmount;

	auto startTime = std::chrono::high_resolution_clock::now();
	double outputInterval = 10.0;
	auto lastOutputTime = startTime;
	produce(fuelIndex, 1, chemicalStore, reactions);

	auto oreUsedFor1Fuel = initialOreAmount - chemicalStore[oreIndex];
	size_t totalFuelProduced = 0;
	// Naive brute force production
	while (produce(fuelIndex, 1, chemicalStore, reactions))
	{
		totalFuelProduced += chemicalStore[fuelIndex];
		chemicalStore[fuelIndex] = 0;

		auto now = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> totalDuration = now - startTime;
		std::chrono::duration<double> timeSinceLastOutput = now - lastOutputTime;
		if (timeSinceLastOutput.count() >= outputInterval)
		{
			uint64_t oreLeftInStore = chemicalStore[oreIndex];
			std::cout << "Ore left: " << oreLeftInStore << ", Fuel produced: " << totalFuelProduced << ", Fuel/s: " << totalFuelProduced / totalDuration.count() << std::endl;
			lastOutputTime = now;
		}
	}

	auto endTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> duration = endTime - startTime;

	std::cout << "Part 1: " << oreUsedFor1Fuel << std::endl;
	std::cout << "Part 2: " << totalFuelProduced << std::endl;
	std::cout << "Time taken: " << duration.count() << std::endl;
}
