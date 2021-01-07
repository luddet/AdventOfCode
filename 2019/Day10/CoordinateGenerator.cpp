#include "CoordinateGenerator.h"
namespace Day10
{

point_t CoordinateGenerator::getNext()
{
	m_currentCoordinate = m_currentCoordinate + m_offsets[(int)m_currentDirection];
	++m_currentSegmentPosition;
	if (m_currentSegmentPosition >= m_currentSegmentLength)
	{
		m_currentSegmentPosition = 0;
		switch (m_currentDirection)
		{
			case direction_t::DOWN:
			case direction_t::UP:
				m_currentSegmentLength++;
		}
		m_currentDirection = direction_t(((int)m_currentDirection + 1) % (int)direction_t::DIRECTION_COUNT);
	}
	return m_currentCoordinate;
}

} // namespace Day10