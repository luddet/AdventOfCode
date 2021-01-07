// Day01.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <exception>
#include <cassert>

int findProduct(std::vector<int>& numbers, int sum)
{
	for (size_t i = 0; i < numbers.size() - 1; ++i)
		for (size_t j = i + 1; j < numbers.size(); ++j)
			if (numbers[i] + numbers[j] == sum)
				return numbers[i] * numbers[j];
			
	throw std::exception("Failed to find match.");
}

int findTripleProduct(std::vector<int>& numbers, int sum)
{
	for (size_t i = 0; i < numbers.size() - 2; ++i)
		for (size_t j = i + 1; j < numbers.size() - 1; ++j)
			for (size_t k = j + 1; k < numbers.size(); ++k)
				if (numbers[i] + numbers[j] + numbers[k] == sum)
					return numbers[i] * numbers[j] * numbers[k];

	throw std::exception("Failed to find match.");
}

std::vector<int> getInputVector(std::istream& is)
{
	std::vector<int> v;
	int n;
	while (is >> n)
		v.push_back(n);
	return v;
}

int main()
{
	constexpr int SUM(2020);
	constexpr int TEST_ANSWER_1(514579);
	constexpr int TEST_ANSWER_2(241861950);

	std::istringstream testInput(std::string("1721\n979\n366\n299\n675\n1456"));
	auto testNumbers = getInputVector(testInput);
	int testProduct1 = findProduct(testNumbers, SUM);
	assert(testProduct1 == TEST_ANSWER_1);

	int testProduct2 = findTripleProduct(testNumbers, SUM);
	assert(testProduct2 == TEST_ANSWER_2);

	std::ifstream input("input.txt");
	auto numbers = getInputVector(input);

	std::cout << "Part 1: " << findProduct(numbers, SUM) << std::endl;
	std::cout << "Part 2: " << findTripleProduct(numbers, SUM) << std::endl;


}
