#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>

using IntType = size_t;
using TransformFunc = std::function<IntType(IntType)>;
using IndexFunc = std::function<size_t(IntType)>;
using ItemsContainer = std::vector<IntType>;

IntType calculate(std::vector<ItemsContainer> items, const std::vector<IndexFunc>& indexFunctions, const std::vector<TransformFunc>& ops, int rounds, TransformFunc divider)
{
	std::vector<size_t> inspections(items.size(), 0);

	for (size_t round = 0; round < rounds; ++round)
	{
		for (size_t monkey = 0; monkey < items.size(); ++monkey)
		{
			inspections[monkey] += items[monkey].size();
			for (auto& currentItem : items[monkey])
			{
				currentItem = ops[monkey](currentItem);
				currentItem = divider(currentItem);
				items[indexFunctions[monkey](currentItem)].push_back(currentItem);
			}
			items[monkey].clear();
		}
	}

	std::sort(begin(inspections), end(inspections), std::greater<>());
	return inspections[0] * inspections[1];
}

int main()
{
	std::vector<ItemsContainer> items{
		{66, 59, 64, 51},
		{67, 61},
		{86, 93, 80, 70, 71, 81, 56},
		{94},
		{71, 92, 64},
		{58, 81, 92, 75, 56},
		{82, 98, 77, 94, 86, 81},
		{54, 95, 70, 93, 88, 93, 63, 50}
	};
	
	std::vector<TransformFunc> ops{
		[](auto n) { return n * 3u; }, // 0
		[](auto n) { return n * 19u; },// 1
		[](auto n) { return n + 2u; }, // 2
		[](auto n) { return n * n; }, // 3
		[](auto n) { return n + 8u; }, // 4
		[](auto n) { return n + 6u; }, // 5
		[](auto n) { return n + 7u; }, // 6
		[](auto n) { return n + 4u; }, // 7
	};

	std::vector<IndexFunc> indexFunctions{
		[](auto n) { return (n % 2 == 0) ? 1 : 4; }, // 0
		[](auto n) { return (n % 7 == 0) ? 3 : 5; }, // 1
		[](auto n) { return (n % 11 == 0) ? 4 : 0; },// 2
		[](auto n) { return (n % 19 == 0) ? 7 : 6; },// 3
		[](auto n) { return (n % 3 == 0) ? 5 : 1; }, // 4
		[](auto n) { return (n % 5 == 0) ? 3 : 6; }, // 5
		[](auto n) { return (n % 17 == 0) ? 7 : 2; },// 6
		[](auto n) { return (n % 13 == 0) ? 2 : 0; } // 7
	};

	constexpr const static auto product = 2 * 7 * 11 * 19 * 3 * 5 * 17 * 13;

	auto part1 = calculate(items, indexFunctions, ops, 20, [](auto n) {return n / 3; });
	auto part2 = calculate(items, indexFunctions, ops, 10000, [](auto n) { return n % product; });

	std::cout << "Day11 Part 1: " << part1 << '\n';
	std::cout << "Day11 Part 2: " << part2 << '\n';
}
