#pragma once
#include <istream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <ranges>
#include <vector>

#include "dllexport.h"


template<typename T>
auto readItems(std::istream& is, const char delimiter = ' ')
{
	std::vector<T> result;
	while (!is.eof())
	{
		T i;
		is >> i;
		if (is.bad())
			break;
		
		if (is.fail())
		{
			is.clear();
			is.ignore( (std::numeric_limits<std::streamsize>::max)(), delimiter);
			continue;
		}

		result.push_back(i);
	}
	return result;
}

template<typename T>
auto readItems(std::string& str, const char delimiter = ' ')
{
	std::istringstream ss(str);
	return readItems<T>(ss, delimiter);
}

DLLEXPORT std::vector<int> readInts(std::istream& os);

DLLEXPORT std::vector<std::string> readLines(const std::string& filePath);
DLLEXPORT std::vector<std::string> readLines(std::istream& is);
DLLEXPORT std::vector<std::string> readLines(std::istream&& is);

template<class SetType, class... Pack>
SetType intersectSets(SetType& set0, Pack&... pack)
{
	static_assert(sizeof...(pack) > 0);
	SetType result{ set0 }, temp{};
	((std::ranges::set_intersection(result, pack, std::inserter(temp, temp.begin())), result = temp, temp.clear()), ...);
	return result;
}

template<class RangeType> requires std::ranges::range<RangeType>
auto intersectRangeOfSets(const RangeType& container) -> std::ranges::range_value_t<decltype(container)>
{
	std::ranges::range_value_t<decltype(container)> result{ *begin(container) }, temp{};
	std::ranges::for_each(container | std::views::drop(1), [&](const auto& s) {
		std::ranges::set_intersection(result, s, std::inserter(temp, begin(temp))), result = temp, temp.clear();
	});
	return result;
}
