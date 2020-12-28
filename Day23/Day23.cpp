#include <iostream>
#include <vector>
#include <string>
#include <array>
#include <cassert>
#include <algorithm>

const std::vector<uint32_t> INPUT = { 7,1,6,8,9,2,5,4,3 };
const std::vector<uint32_t> EXAMPLE = { 3,8,9,1,2,5,4,6,7 };

template<size_t NUM>
void print(const std::array<size_t, NUM>& cups, size_t start, size_t current)
{
	size_t i = start;
	do
	{
		std::cout << (i == current ? "(": "") << (i+1) << (i==current ? ") " : " ");
		i = cups[i];
	} while (i != start);
	std::cout << std::endl;
}

template<size_t NUM>
void move(std::array<size_t, NUM>& cups, size_t beforeStart, size_t end, size_t dest)
{
	size_t temp = cups[dest];
	cups[dest] = cups[beforeStart];
	cups[beforeStart] = cups[end];
	cups[end] = temp;
}

template<size_t NUM>
size_t step(std::array<size_t, NUM>& cups, size_t current, size_t steps)
{
	size_t result{ current };
	for (size_t i = 0; i < steps; ++i)
		result = cups[result];
	return result;
}

template<size_t NUM, size_t NUM_RESULT>
std::array<size_t, NUM_RESULT> getStepArray(std::array<size_t, NUM>& cups, size_t current)
{
	std::array<size_t, NUM_RESULT> result;
	result[0] = cups[current];
	for (size_t i = 1; i < NUM_RESULT; ++i)
		result[i] = cups[result[i-1]];
	return result;
}

template<size_t NUM_CUPS>
std::array<size_t, NUM_CUPS> execute(const std::vector<uint32_t>& seed, const size_t iterations)
{
	constexpr size_t NUM_PICKUPS{ 3 };
	assert(NUM_CUPS >= seed.size());
	std::array<size_t, NUM_CUPS> cups{};

	size_t current{ seed[0] - 1 };
	size_t start{ seed[0] - 1 };

//	// copy seed and fill rest incrementally
	for (size_t i = 0; i < seed.size(); ++i)
		cups[seed[i]-1] = seed[(i + 1) % seed.size()]-1;

	if (NUM_CUPS > seed.size())
	{
		size_t lastSeed = seed.back() - 1;
		size_t seedMax = *std::max_element(seed.begin(), seed.end());
		size_t nextIndex{ seedMax };
		cups[lastSeed] = nextIndex;
		std::generate(cups.begin() + nextIndex, cups.end(), [&]() { return (++nextIndex < NUM_CUPS) ? nextIndex : start; });
	}

	for (size_t iteration = 1; iteration <= iterations; ++iteration)
	{
		size_t destination = (current > 0) ? current - 1 : NUM_CUPS - 1;
		auto pickups = getStepArray<NUM_CUPS, NUM_PICKUPS>(cups, current);

		while (std::find(pickups.begin(), pickups.end(), destination) != pickups.end())
			destination = (destination > 0) ? destination - 1 : NUM_CUPS - 1;

		move(cups, current, step(cups, current, NUM_PICKUPS), destination);
		current = cups[current];
	}
	return cups;
}

int main()
{
	auto& in = INPUT;
	auto cups1 = execute<9>(in, 100);

	std::cout << "Day23 Part 1: ";
	auto current = cups1[0];
	while (current != 0)
	{
		std::cout << current + 1;
		current = cups1[current];
	}
	std::cout << std::endl;

	auto cups2 = execute<1000000>(in, 10000000);

	uint64_t first = cups2[0]+1;
	uint64_t second = cups2[first-1]+1;
	std::cout << "Day23 Part 2: " << first * second << std::endl;
}