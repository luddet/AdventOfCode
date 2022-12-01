#include <array>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <string>

using namespace std;

template<class T, class Container, class Comp>
void heap_insert(T value, Container& c, Comp comparer)
{
	*(end(c) - 1) = value;
	push_heap(begin(c), end(c), comparer);
	pop_heap(begin(c), end(c), comparer);
}

int main()
{
	array<int,4> topCals {};
	int currentCalories {};
	ifstream file{ "input.txt" };
	string line;

	while (getline(file, line))
	{
		if (line.size() == 0)
		{
			heap_insert(currentCalories, topCals, greater<int>());
			currentCalories = 0;
		}
		else
			currentCalories += atoi(line.c_str());
	}

	cout << "Day01 Part 1: " << *max_element(begin(topCals), begin(topCals)+3) << '\n';
	cout << "Day01 Part 2: " << accumulate(begin(topCals), begin(topCals)+3, 0) << '\n';
}
