// https://adventofcode.com/2019/day/16

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <functional>
#include <chrono>

const char* EXAMPLE1 = "19617804207202209144916044189917";

std::ostream& operator<<(std::ostream& os, const std::vector<uint8_t>& v)
{
	for (auto c : v)
		os << c;
	return os;
}

std::vector<uint8_t> transform(const std::vector<uint8_t>& list, size_t iterations)
{
	std::vector basePattern = { 0, 1, 0, -1 };

	std::vector<uint8_t> listA(list.size());
	std::vector<uint8_t> listB(list.begin(), list.end());
	auto* current = &listA;
	auto* old = &listB;

	auto startTime = std::chrono::steady_clock::now();
	auto lastPrint = startTime;
	for (size_t iteration = 0; iteration < iterations; ++iteration)
	{
		for (size_t i = 0; i < current->size(); ++i)
		{
			int64_t newValue{ 0 };
			for (size_t j = 0; j < current->size(); ++j)
			{
				auto basePatternIndex = ((j + 1) / (i + 1)) % 4;
				newValue += int64_t((*old)[j]) * basePattern[basePatternIndex];


			}

			if (i % 10 == 0)
			{
				auto now = std::chrono::steady_clock::now();
				auto timeSinceLast = std::chrono::duration<double>(now - lastPrint).count();
				auto timeSinceStart = std::chrono::duration<double>(now - startTime).count();

				if (timeSinceLast > 10)
				{
					std::cout << "iteration: " << iteration 
						<< ", i: " << i 
						<< ", time/i (s): " << (timeSinceStart/i) 
						<< ", time/iteration (s): " << timeSinceStart/(double_t(i)/current->size()) << std::endl;
					lastPrint = now;

				}
			}

			(*current)[i] = uint8_t(abs(newValue % 10));
		}
		std::swap(current, old);

	}
	return *old;
}

int main()
{
	//std::istringstream is(EXAMPLE1);
	std::ifstream is("input.txt");

	std::string input;
	std::getline(is, input);

	std::vector<uint8_t> list;
	list.reserve(input.size());
	for (auto c : input)
		list.push_back(c - '0');

	auto part1Transformed = transform(list, 100);

	std::vector<uint8_t> finalList{};
	std::transform(part1Transformed.begin(), part1Transformed.begin() + 8, std::back_inserter(finalList), [](uint8_t e) { return uint8_t(e + '0'); });

	const size_t repeats{ 10000 };
	std::vector<uint8_t> part2Input(list.size() * repeats);
	auto dest = part2Input.begin();
	for (size_t i = 0; i < repeats; ++i)
		dest = std::copy(list.begin(), list.end(), dest);

	auto part2Transformed = transform(part2Input, 100);

	std::cout << "Day16 Part1: " << finalList << std::endl;
	std::cout << "Day16 Part2: " << std::endl;
}
