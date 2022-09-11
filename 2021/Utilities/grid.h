#pragma once
#include <array>
#include <exception>

template<typename T, size_t W, size_t H>
class Grid
{
public:
	constexpr static const size_t Width = W;
	constexpr static const size_t Height = H;
	constexpr static const size_t Size = Width * Height;

	constexpr Grid(const std::array<T, Size>&& values) noexcept
		: m_grid(std::move(values))
	{}

	constexpr const T& at(const size_t x, const size_t y) const
	{
		return m_grid[y * Width + x];
	}

	constexpr T& at(const size_t x, const size_t y)
	{
		return m_grid[y * Width + x];
	}

private:
	std::array<T, Size> m_grid{};
};

template<typename T>
consteval std::pair<std::string_view, T> extract(const std::string_view str)
{
	return { {}, T{} };
}

template<>
consteval std::pair<std::string_view, char> extract(const std::string_view str)
{
	auto first = str.begin();
	while (*first == '\n')
		++first;
	if (first == str.end())
		throw std::invalid_argument("Unable to extract char before reaching end of str.");

	return { {first+1, str.end()}, *first};
}

template<typename T, size_t Width, size_t Height>
consteval Grid<T, Width, Height> readGrid(const std::string_view str)
{
	constexpr size_t size = Width * Height;
	std::array<T, size> data{};
	size_t i{ 0 };

	auto remainingStr{ str };
	while (!remainingStr.empty())
	{
		constexpr auto result = extract<T>(remainingStr);
		remainingStr = result.first;
		data[i++] = std::move(result.second);
	}
	if (i != size)
		throw std::invalid_argument("Wrong number of characters.");

	return Grid<T, Width, Height>{std::move(data)};
}

