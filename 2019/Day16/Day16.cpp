// https://adventofcode.com/2019/day/16

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <functional>
#include <numeric>
#include <chrono>

const char* EXAMPLE1 = "12345678";

template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v)
{
	size_t i = 0;
	for (auto c : v)
		os << c << ((++i % 10 == 0) ? " " : "");
	return os;
}

std::vector<int64_t> transform(const std::vector<uint8_t>& list, size_t iterations)
{
	std::vector basePattern = { 0, 1, 0, -1 };
	const size_t patternSize = basePattern.size();
	const int64_t listSize = list.size();
	std::vector<int64_t> listA(listSize);
	std::vector<int64_t> listB(list.begin(), list.end());
	auto* current = &listA;
	auto* old = &listB;

	std::vector<int64_t> factor{ 1,-1,-1, 1};
	auto startTime = std::chrono::steady_clock::now();
	auto lastPrint = startTime;
	for (size_t iteration = 0; iteration < iterations; ++iteration)
	{
		std::fill(current->begin(), current->end(), 0);
		current->back() = old->back();
		for (int64_t outIndex = listSize - 2; outIndex >= 0; --outIndex)
			(*current)[outIndex] = (*old)[outIndex] + (*current)[outIndex+1];

		int64_t factorIndex = 1;
		int64_t step = 2;
		for (int64_t start = listSize - 1; start >= 0; start /= 2)
		{
			int64_t currentPartial{ 0 };
			for (int64_t outIndex = start-1; outIndex >= 0; --outIndex)
			{
				int64_t oldStart = (outIndex+1) * step - 1;
				int64_t oldIndex = std::min(oldStart + step - 1, listSize - 1);
				currentPartial += factor[factorIndex] * (*old)[oldIndex--];

				for (; oldIndex >= oldStart; --oldIndex)
					currentPartial += factor[factorIndex] * (*old)[oldIndex];
				(*current)[outIndex] += currentPartial;
			}
			factorIndex = ++factorIndex % 4;
			++step;
		}

		std::swap(current, old);

		auto now = std::chrono::steady_clock::now();
		[[maybe_unused]]
		auto timeSinceLast = std::chrono::duration<double>(now - lastPrint).count();

		[[maybe_unused]]
		auto timeSinceStart = std::chrono::duration<double>(now - startTime).count();

		//if (timeSinceLast > 1)
		{
			//std::cout << "iteration: " << iteration
			//	<< ", time/iteration (s): " << timeSinceStart / double_t(iteration+1) << std::endl;
			std::cout << *old << std::endl;
			lastPrint = now;

		}
	}
	return *old;
}

int main()
{
	std::istringstream is(EXAMPLE1);
	//std::ifstream is("input.txt");

	std::string input;
	std::getline(is, input);

	std::vector<uint8_t> list;
	list.reserve(input.size());
	for (auto c : input)
		list.push_back(c - '0');

	//auto part1Transformed = transform(list, 100);

	//std::vector<int32_t> finalList{};
	//std::transform(part1Transformed.begin(), part1Transformed.begin() + 8, std::back_inserter(finalList), [](auto e) { return e + '0'; });

	const size_t repeats{ 1 };
	std::vector<uint8_t> part2Input(list.size() * repeats);
	auto dest = part2Input.begin();
	for (size_t i = 0; i < repeats; ++i)
		dest = std::copy(list.begin(), list.end(), dest);

	std::cout << std::vector<int32_t>(part2Input.begin(), part2Input.end()) << std::endl;
	auto part2Transformed = transform(part2Input, 100);
	
	size_t offset = 0;
	for (size_t i = 0; i < 7; ++i)
		offset += size_t(std::pow(10, i)) * part2Transformed[6 - i];

	std::vector<int32_t> part2Final{};
	//std::transform(part2Transformed.begin() + offset, part2Transformed.begin() +offset + 8, std::back_inserter(part2Final), [](auto e) { return e + '0'; });



	//std::cout << "Day16 Part1: " << finalList << std::endl;
	//std::cout << "Day16 Part2: " << part2Final << std::endl;
}
