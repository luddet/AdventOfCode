#include <algorithm>
#include <array>
#include <cassert>
#include <chrono>
#include <deque>
#include <execution>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <vector>

#pragma warning(push)
#pragma warning(disable: 4127)
#pragma warning(disable: 4146)
#pragma warning(disable: 4244)
#include <gmpxx.h>
#pragma warning(pop)


constexpr bool OVERFLOWCHECK{ false };

using IntType = mpz_class;
using Func = std::function<IntType(const IntType&)>;
using ReceiverFunc = std::function<size_t(const IntType&)>;
using Items = std::vector<IntType>;

IntType calculate1(const std::vector<Items>& item_vector, const std::vector<ReceiverFunc>& tests, const std::vector<Func>& ops, int rounds)
{
	std::vector<Items> items(item_vector);
	std::vector<int> inspections(items.size(), 0);

	auto start = std::chrono::high_resolution_clock::now();

	for (size_t round = 0; round < rounds; ++round)
	{
		auto roundStart = std::chrono::high_resolution_clock::now();

		std::cout << "Round " << std::setw(4) << std::setfill('0') << (round + 1) << " ...";
		for (size_t monkey = 0; monkey < items.size(); ++monkey)
		{
			for (auto& currentItem : items[monkey])
			{
				++inspections[monkey];

				currentItem = ops[monkey](currentItem);

				currentItem /= 3;

				auto receiver = tests[monkey](currentItem);
				items[receiver].push_back(currentItem);
			}
			items[monkey].clear();
		}

		auto roundEnd = std::chrono::high_resolution_clock::now();
		std::cout << " done. ";
		std::cout << "Round time: " << std::chrono::duration<float>(roundEnd - roundStart).count() << " s. ";
		std::cout << "Total time: " << std::chrono::duration<float>(roundEnd - start).count() << " s.\n";
	}

	std::sort(begin(inspections), end(inspections), std::greater<>());
	return inspections[0] * inspections[1];
}

size_t calculate2(const std::vector<Items>& item_vector, const int rounds)
{
	std::vector<Items> items(item_vector);
	std::vector<size_t> inspections(items.size(), 0);
	
	auto start = std::chrono::high_resolution_clock::now();

	for (size_t round = 0; round < rounds; ++round)
	{
		auto roundStart = std::chrono::high_resolution_clock::now();

		std::cout << "Round " << std::setw(4) << std::setfill('0') << (round+1) << " ...";
		
		for (size_t monkey = 0; monkey < items.size(); ++monkey)
		{
			if (!items[monkey].empty())
			{
				inspections[monkey] += items[monkey].size();

				std::vector<std::pair<size_t, IntType>> transformed(items[monkey].size(), std::make_pair(0, 0));
				std::transform(std::execution::par_unseq, begin(items[monkey]), end(items[monkey]), begin(transformed), [monkey](auto& currentItem) -> std::pair<size_t, IntType>
				{
					switch (monkey)
					{
						case 0:
							currentItem *= 3u;
							break;
						case 1:
							currentItem *= 19u;
							break;
						case 2:
							currentItem += 2u;
							break;
						case 3:
							currentItem *= currentItem;
							break;
						case 4:
							currentItem += 8u;
							break;
						case 5:
							currentItem += 6u;
							break;
						case 6:
							currentItem += 7u;
							break;
						case 7:
							currentItem += 4u;
							break;
						default:
							assert(false && "Noooooo");
					};

					constexpr const static auto product = 2 * 7 * 11 * 19 * 3 * 5 * 17 * 13;
					currentItem = currentItem % product;

					size_t receiver{ 0 };
					switch (monkey)
					{
						case 0:
							receiver = (currentItem % 2 == 0) ? 1 : 4;
							break;
						case 1:
							receiver = (currentItem % 7 == 0) ? 3 : 5;
							break;
						case 2:
							receiver = (currentItem % 11 == 0) ? 4 : 0;
							break;
						case 3:
							receiver = (currentItem % 19 == 0) ? 7 : 6;
							break;
						case 4:
							receiver = (currentItem % 3 == 0) ? 5 : 1;
							break;
						case 5:
							receiver = (currentItem % 5 == 0) ? 3 : 6;
							break;
						case 6:
							receiver = (currentItem % 17 == 0) ? 7 : 2;
							break;
						case 7:
							receiver = (currentItem % 13 == 0) ? 2 : 0;
							break;
					}

					return { receiver, currentItem };
				});

				std::for_each(cbegin(transformed), cend(transformed), [&items](const auto& t) 
				{
					items[t.first].push_back(t.second);
				});

				items[monkey].clear();
			}
		}

		auto roundEnd = std::chrono::high_resolution_clock::now();
		std::cout << " done. ";
		std::cout << "Round time: " << std::chrono::duration<float>(roundEnd - roundStart).count() << " s. ";
		std::cout << "Total time: " << std::chrono::duration<float>(roundEnd - start).count() << " s.\n";
	}

	std::sort(begin(inspections), end(inspections), std::greater<>());
	return inspections[0] * inspections[1];
}

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

	std::vector<Func> ops{
		[](const IntType& n) -> IntType { return n * 3u; }, // 0
		[](const IntType& n) -> IntType { return n * 19u; },// 1
		[](const IntType& n) -> IntType { return n + 2u; }, // 2
		[](const IntType& n) -> IntType { return n * n; }, // 3
		[](const IntType& n) -> IntType { return n + 8u; }, // 4
		[](const IntType& n) -> IntType { return n + 6u; }, // 5
		[](const IntType& n) -> IntType { return n + 7u; }, // 6
		[](const IntType& n) -> IntType { return n + 4u; }, // 7
	};

	std::vector<ReceiverFunc> tests{
		[](const IntType& n) -> size_t { return (n % 2 == 0) ? 1 : 4; }, // 0
		[](const IntType& n) -> size_t { return (n % 7 == 0) ? 3 : 5; }, // 1
		[](const IntType& n) -> size_t { return (n % 11 == 0) ? 4 : 0; },// 2
		[](const IntType& n) -> size_t { return (n % 19 == 0) ? 7 : 6; },// 3
		[](const IntType& n) -> size_t { return (n % 3 == 0) ? 5 : 1; }, // 4
		[](const IntType& n) -> size_t { return (n % 5 == 0) ? 3 : 6; }, // 5
		[](const IntType& n) -> size_t { return (n % 17 == 0) ? 7 : 2; },// 6
		[](const IntType& n) -> size_t { return (n % 13 == 0) ? 2 : 0; } // 7
	};

	auto part1 = calculate1(items, tests, ops, 20);
	auto part2 = calculate2(items, 10000);

	std::cout << "Day11 Part 1: " << part1 << '\n';
	std::cout << "Day11 Part 2: " << part2 << '\n';
}
