#pragma once
#include <array>
#include <exception>
#include <istream>
#include <sstream>

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


template<size_t Width, size_t Height>
consteval auto readGrid(const std::string_view str) -> Grid<char, Width, Height>
{
	constexpr size_t size = Width * Height;
	std::array<char, size> data{};
	size_t i{ 0 };
	for (auto c : str)
	{
		if (c == '\n')
		{
			if (i % Width != 0)
				throw std::invalid_argument("Wrong number of characters.");
			continue;
		}

		data[i++] = c;
	}
	if (i != size)
		throw std::invalid_argument("Wrong number of characters.");

	return Grid<char, Width, Height>{std::move(data)};
}

