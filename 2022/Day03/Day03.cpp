#include <fstream>
#include <iostream>
#include <set>
#include <algorithm>
#include <ranges>
#include <concepts>

#include "../../2021/Utilities/utilities.h"

int getPriority(auto c) requires std::integral<decltype(c)> {	return c >= 'A' && c <= 'Z' ? c - 'A' + 27 : c - 'a' + 1; }

int main()
{
	using namespace std::views;

	auto lines = readLines(std::ifstream("input.txt"));
	int part1{ 0 };
	for (auto& line : lines)
	{
		std::set<unsigned char> c1, c2;
		size_t halfLen{ line.size() / 2 };
		for (auto c : line | take(halfLen))
			c1.insert(c);
		for (auto c : line | drop(halfLen))
			c2.insert(c);

		part1 += getPriority(*begin(intersectSets(c1, c2)));
	}

	int part2{ 0 };
	for (auto group : lines | chunk(3))
		part2 += getPriority(*begin(intersectRangeOfSets(group | transform([](const auto& str) { return std::set(begin(str), end(str)); }))));
	
	std::cout << "Day03 Part 1: " << part1 << '\n';
	std::cout << "Day03 Part 2: " << part2 << '\n';
}
