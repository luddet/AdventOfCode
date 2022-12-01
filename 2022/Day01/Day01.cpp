#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <numeric>
#include <ranges>
#include <string>

using namespace std;

int main()
{
	array<int,3> topCals {};
	int currentCalories {};
	ifstream file{ "input.txt" };
	string line;

	while (getline(file, line))
	{
		if (line.size() == 0)
		{
			auto minIt = min_element(begin(topCals), end(topCals));
			*minIt = max(currentCalories, *minIt);
			currentCalories = 0;
		}
		else
			currentCalories += atoi(line.c_str());
	}

	cout << "Day01 Part 1: " << ranges::max(topCals) << '\n';
	cout << "Day01 Part 2: " << accumulate(begin(topCals), end(topCals), 0) << '\n';
}
