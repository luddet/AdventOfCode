#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <numeric>
#include <ranges>
#include <string>

using namespace std;

istream& sumGroups(istream& s, int& out)
{
	out = 0;
	string line;
	while (getline(s, line) && line.size() != 0)
		out += atoi(line.c_str());
	return s;
}

template<class Container>
void replace_min(Container& c, typename Container::value_type v)
{
	auto minIt = min_element(begin(c), end(c));
	*minIt = max(v, *minIt);
}

int main()
{
	ifstream file{ "input.txt" };
	
	array<int,3> topCals {};
	int calories {};

	while (sumGroups(file, calories))
		replace_min(topCals, calories);

	cout << "Day01 Part 1: " << ranges::max(topCals) << '\n';
	cout << "Day01 Part 2: " << accumulate(begin(topCals), end(topCals), 0) << '\n';
}
