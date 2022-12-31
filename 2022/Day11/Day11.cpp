#include <algorithm>
#include <deque>
#include <functional>
#include <iostream>
#include <vector>

using OpFunc = std::function<int(int)>;
using TestFunc = std::function<size_t(int)>;
using Items = std::deque<int>;

int main()
{
	std::vector<Items> items{
		{66, 59, 64, 51},
		{67, 61},
		{86, 93, 80, 70, 71, 81, 56},
		{94},
		{71, 92, 64},
		{58, 81, 92, 75, 56},
		{82, 98, 77, 94, 86, 81},
		{54, 95, 70, 93, 88, 93, 63, 50}
	};

	std::vector<OpFunc> ops{
		[](int n) { return n * 3; },
		[](int n) { return n * 19; },
		[](int n) { return n + 2; },
		[](int n) { return n * n; },
		[](int n) { return n + 8; },
		[](int n) { return n + 6; },
		[](int n) { return n + 7; },
		[](int n) { return n + 4; },
	};

	std::vector<TestFunc> tests{
		[](int n) { return (n % 2 == 0) ? 1 : 4; },
		[](int n) { return (n % 7 == 0) ? 3 : 5; },
		[](int n) { return (n % 11 == 0) ? 4 : 0; },
		[](int n) { return (n % 19 == 0) ? 7 : 6; },
		[](int n) { return (n % 3 == 0) ? 5 : 1; },
		[](int n) { return (n % 5 == 0) ? 3 : 6; },
		[](int n) { return (n % 17 == 0) ? 7 : 2; },
		[](int n) { return (n % 13 == 0) ? 2 : 0; }
	};
	
	std::vector<int> inspections(items.size(), 0);

	for (size_t round = 0; round < 20; ++round)
	{
		for (int monkey = 0; monkey < items.size(); ++monkey)
		{
			while (!items[monkey].empty())
			{
				auto currentItem = items[monkey].front();
				items[monkey].pop_front();

				++inspections[monkey];

				currentItem = ops[monkey](currentItem);
				currentItem /= 3;
				auto receiver = tests[monkey](currentItem);
				items[receiver].push_back(currentItem);
			}
		}
	}

	std::sort(begin(inspections), end(inspections), std::greater<>());
	auto part1 = inspections[0] * inspections[1];

	std::cout << "Day11 Part 1: " << part1 << '\n';
	std::cout << "Day11 Part 2: " << '\n';
}
