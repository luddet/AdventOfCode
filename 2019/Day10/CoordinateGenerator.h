#pragma once
#include "Day10.h"
#include <array>

namespace Day10
{
class CoordinateGenerator
{
public:
	CoordinateGenerator(point_t origin) :
		m_currentCoordinate(origin),
		m_offsets{ point_t{1, 0}, point_t{0, 1}, point_t{-1, 0}, point_t{0, -1} },
		m_currentSegmentLength(1),
		m_currentSegmentPosition(0),
		m_currentDirection(direction_t::RIGHT)
	{};
	point_t getNext();
private:
	enum class direction_t
	{
		RIGHT = 0,
		DOWN,
		LEFT,
		UP,
		DIRECTION_COUNT
	};
	std::array<point_t, 4> m_offsets;
	int m_currentSegmentLength;
	int m_currentSegmentPosition;
	direction_t m_currentDirection;
	point_t m_currentCoordinate;
};

} // namespace Day10
