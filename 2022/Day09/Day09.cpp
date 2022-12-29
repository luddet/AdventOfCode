#include <cmath>
#include <iostream>
#include <tuple>
#include <unordered_set>
#include <utility>
#include <type_traits>
#include "../../2021/Utilities/utilities.h"

constexpr size_t X = 0;
constexpr size_t Y = 1;

using Point = std::tuple<int, int>;

template<class... Types>
struct std::hash<std::tuple<Types...>>
{
	std::size_t operator()(const std::tuple<Types...>& t) const noexcept
	{
		return internalHash(t, std::index_sequence_for<Types...>{});
	}

private:
	template<class Tuple, size_t... Idx>
	std::size_t internalHash(Tuple t, std::index_sequence<Idx...>) const noexcept
	{
		return ( (std::hash<std::tuple_element_t<Idx, Tuple>>{}(std::get<Idx>(t)) << Idx) ^ ...);
	}
};


int sign(auto n)
{
	static_assert(std::is_arithmetic_v<decltype(n)>, "Type has to be arithmetic.");
	if constexpr (std::is_unsigned_v<decltype(n)>)
		return n > 0 ? 1 : 0;
	else
		return n > 0 ? 1 : n < 0 ? -1 : 0;
}

Point operator-(const Point& lhs, const Point& rhs)
{
	using std::get;
	return { get<X>(lhs) - get<X>(rhs), get<Y>(lhs) - get<Y>(rhs) };
}

bool isAdjacent(const Point& a, const Point& b)
{
	using std::get;
	auto d = a - b;
	return std::abs(get<X>(d)) <= 1 && std::abs(get<Y>(d)) <= 1;
}

Point stepTowards(const Point& target, const Point& current)
{
	using std::get;
	const auto diff = target - current;
	const auto sx{ sign(get<X>(diff)) };
	const auto sy{ sign(get<Y>(diff)) };
	return { get<X>(current) + sx, get<Y>(current) + sy};
}

int main()
{
	auto input = split(getInput(2022, 9));

	Point head{ 0,0 }, tail{ 0,0 };
	std::unordered_set<Point> tailPositions;
	tailPositions.insert(tail);

	for (const auto& move : input)
	{
		auto count = std::stoi(move.substr(2));
		switch (move[0])
		{
			case 'R':
				std::get<X>(head) += count;
				break;
			case 'L':
				std::get<X>(head) -= count;
				break;
			case 'U':
				std::get<Y>(head) -= count;
				break;
			case 'D':
				std::get<Y>(head) += count;
				break;
		}

		while (!isAdjacent(head, tail))
		{
			tail = stepTowards(head, tail);
			tailPositions.insert(tail);
		}
	}

	auto part1 = tailPositions.size();

	std::cout << "Day09 Part 1: " << part1 << '\n';
	std::cout << "Day09 Part 2: " << '\n';
}
