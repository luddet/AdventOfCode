#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

#include "../Utilities/utilities.h"

auto readTokens(std::istream& is, auto& container, auto num)
{
	std::vector<std::string> input;
	for (size_t i = 0; i < num; ++i)
	{
		std::string token;
		is >> token;
		if (token == "")
			return;
		input.push_back(std::move(token));
	}
	container.push_back(std::move(input));
};


int main()
{
	std::ifstream ifs("input.txt");
	
	std::vector<std::vector<std::string>> inputs;
	std::vector<std::vector<std::string>> outputs;

	while (ifs)
	{
		readTokens(ifs, inputs, 10);
		std::string _; ifs >> _; // discard |
		readTokens(ifs, outputs, 4);
	}

	auto count = std::accumulate(begin(outputs), end(outputs), 0ll, [](auto acc, auto& output)
	{
		return acc + std::count_if(begin(output), end(output), [](auto& str) 
		{
			auto len = str.length();
			return len == 2 || len == 3 || len == 4 || len == 7;
		});
	});

	std::cout << "Day08 Part 1: " << count << '\n';
	std::cout << "Day08 Part 2: " << '\n';
}
