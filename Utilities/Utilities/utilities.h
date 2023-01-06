#pragma once
#include <istream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <ranges>
#include <vector>
#include <functional>

#include "dllexport.h"

class env_error : public std::runtime_error
{
	using std::runtime_error::runtime_error;
};

/// <summary>
/// Generic variadic hasher for std::tuple. Enables hashing for all tuples where all components have std::hash defined.
/// </summary>
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
		return ((std::hash<std::tuple_element_t<Idx, Tuple>>{}(std::get<Idx>(t)) << Idx) ^ ...);
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


struct finally_t : public std::function<void()>
{
	finally_t(auto&& c) :std::function<void()>(std::forward<decltype(c)>(c))
	{
	}
	~finally_t() noexcept { (*this)(); }

	finally_t(const finally_t&) = delete;
	finally_t& operator=(const finally_t&) = delete;
};

finally_t finally(auto&& f)
{
	return finally_t(std::forward<decltype(f)>(f));
}

#define CONCAT_INNER(str1, str2) str1 ## str2
#define CONCAT(str1, str2) CONCAT_INNER(str1, str2)
#define FINALLY(code) auto CONCAT(__finally_, __COUNTER__) = finally([&]{code;})

DLLEXPORT std::string getInput(unsigned int year, unsigned int day, const std::string& cacheFile = "input.txt");

DLLEXPORT std::string getEnvVar(const char* var);
DLLEXPORT std::string getSessionCookie(const std::string& envVar = "AOC_SESSION_COOKIE_FILE");

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

DLLEXPORT std::vector<std::string> split(const std::string& str, const std::string& delims = "\n");


DLLEXPORT std::string readAllText(std::istream& is);
DLLEXPORT std::string readAllText(const std::string& filename);

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
