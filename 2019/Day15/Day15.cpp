// https://adventofcode.com/2019/day/15

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <functional>
#include <random>
#include <stack>
#include <list>
#include <set>
#include <queue>
#include <cassert>
#include "Computer.h"


//template<class T, class Comparator = std::greater<T>>
//class PriorityQueue
//{
//public:
//	PriorityQueue() : m_data{}, m_comp{} {}
//	template<class InputIt>
//	PriorityQueue(InputIt first, InputIt last) : m_data(first, last), m_comp{}
//	{ 
//		std::make_heap(m_data.begin(), m_data.end(), m_comp);
//	}
//
//	template<class UnaryPredicate>
//	bool updateElement(const T& newValue, UnaryPredicate pred)
//	{
//		if (auto elIt = std::find_if(m_data.begin(), m_data.end(), pred); elIt != m_data.end())
//		{
//			*elIt = newValue;
//			std::make_heap(m_data.begin(), m_data.end());
//			return true;
//		}
//		else
//			return false;
//	}
//
//	void push(const T& value) 
//	{ 
//		m_data.push_back(value);
//		std::push_heap(m_data.begin(), m_data.end(), m_comp);
//	}
//
//	T& top() { return m_data.front(); }
//	const T& top() const { return m_data.front(); }
//
//	void pop() 
//	{
//		std::pop_heap(m_data.begin(), m_data.end(), m_comp);
//		m_data.pop_back();
//	}
//
//	bool empty() const { return m_data.empty(); }
//
//private:
//	using container_t = std::vector<T>;
//
//	container_t m_data;
//	Comparator m_comp;
//};

using coord_t = std::pair<int32_t, int32_t>;
enum class TileType : char
{
	Walkable = '.',
	Wall = '#',
	Unexplored = ' '
};

namespace std
{
	template<>
	struct hash<coord_t>
	{
		size_t operator()(const coord_t& coord) const {	return std::hash<int64_t>()(int64_t(coord.first) << 32 | coord.second);	}
	};
}

enum class Direction : int64_t
{
	North = 1,
	South,
	West,
	East
};

coord_t move(const coord_t& pos, Direction dir)
{
	switch (dir)
	{
		case Direction::North:
			return { pos.first, pos.second + 1 };
		case Direction::South:
			return { pos.first, pos.second - 1 };
		case Direction::East:
			return { pos.first + 1, pos.second };
		case Direction::West:
			return { pos.first - 1, pos.second };
		default:
			throw std::invalid_argument("Invalid direction.");
	}
}

Direction reverse(Direction dir)
{
	switch (dir)
	{
		case Direction::North:
			return Direction::South;
		case Direction::South:
			return Direction::North;
		case Direction::East:
			return Direction::West;
		case Direction::West:
			return Direction::East;
		default:
			throw std::invalid_argument("Invalid direction.");
	}
}

int64_t toNum(Direction dir)
{
	return static_cast<int64_t>(dir);
}


using adjGraph_t = std::unordered_map<coord_t, std::list<coord_t>>;
void buildMap(const Computer::mem_container_t& program, coord_t& targetPos, adjGraph_t& adjGraph)
{
	Computer comp(program);
	coord_t currentPos{ 0,0 };
	coord_t lastPos{ 0,0 };
	bool	foundTargetPos{ false };

	std::set<coord_t> visited{};
	std::stack<Direction> directions{};

	directions.push(Direction::North);
	directions.push(Direction::South);
	directions.push(Direction::West);
	directions.push(Direction::East);

	comp.run();
	while (!comp.isDone() && !directions.empty())
	{

		if (comp.getState() != Computer::ComputerState::WAITING_FOR_INPUT)
			throw std::runtime_error("Computer should be waiting for input.");

		auto dir = directions.top();
		directions.pop();

		comp.addInput(toNum(dir));
		comp.run();

		int64_t out;
		comp.getOutput(out);
		if (out == 0)
			continue; // We hit a wall
		assert(out == 1 || out == 2);

		lastPos = currentPos;
		currentPos = move(currentPos, dir);

		if (out == 2)
		{
			foundTargetPos = true;
			targetPos = currentPos;
		}

		adjGraph[lastPos].push_back(currentPos);

		if (visited.find(currentPos) != visited.end())
			continue;

		visited.insert(currentPos);
		auto back = reverse(dir);
		directions.push(back);
		for (auto d : { Direction::North, Direction::South, Direction::East, Direction::West })
			if (d != back)
				directions.push(d);
	}
}

std::vector<coord_t> findShortestPath(coord_t start, coord_t target, const adjGraph_t& adj)
{
	std::set<coord_t> visited{};
	std::unordered_map<coord_t, coord_t> parent{};
	std::unordered_map<coord_t, uint64_t> d{};
	std::queue<coord_t> fringe{};

	fringe.push(start);
	d[start] = 0;

	bool foundPath{ false };
	while (!fringe.empty() && !foundPath)
	{
		auto current = fringe.front();
		fringe.pop();
		visited.insert(current);
		
		size_t currentDelta = d[current];
		for (const auto& n : adj.at(current))
		{
			if (auto nDeltaIt = d.find(n); nDeltaIt == d.end())
			{
				d[n] = currentDelta + 1;
				parent[n] = current;
			}

			if (n == target)
			{
				foundPath = true;
				break;
			}

			if (visited.find(n) == visited.end())
				fringe.push( n );
		}
	}
	std::stack<coord_t> temp{};
	for (auto current = parent.find(target); current != parent.end(); current = parent.find(current->second))
		temp.push(current->first);

	std::vector<coord_t> shortestPath{start};
	shortestPath.reserve(temp.size() + 1);
	while (!temp.empty())
	{
		shortestPath.emplace_back(temp.top());
		temp.pop();
	}
	return shortestPath;
}


size_t findMaxDistance(coord_t start, const adjGraph_t& adj)
{
	std::set<coord_t> visited{};
	std::unordered_map<coord_t, uint64_t> d{};
	std::queue<coord_t> fringe{};

	fringe.push(start);
	d[start] = 0;

	while (!fringe.empty())
	{
		auto current = fringe.front();
		fringe.pop();
		visited.insert(current);

		size_t currentDelta = d[current];
		for (const auto& n : adj.at(current))
		{
			if (auto nDeltaIt = d.find(n); nDeltaIt == d.end())
			{
				d[n] = currentDelta + 1;
			}

			if (visited.find(n) == visited.end())
				fringe.push(n);
		}
	}
	return std::max_element(d.begin(), d.end(),
		[](const auto& a, const auto& b) { return a.second < b.second; })->second;
}


int main()
{
	srand((uint32_t)time(0));
	std::ifstream is("input.txt");
	auto program = readMemory(is);

	adjGraph_t adj{};
	coord_t startPos{ 0,0 };
	coord_t targetPos{};

	buildMap(program, targetPos, adj);
	auto path = findShortestPath(startPos, targetPos, adj);
	auto stepsToOxygen = path.size() - 1;

	auto max = findMaxDistance(targetPos, adj);

	std::cout << "Day15 Part1: " << stepsToOxygen << std::endl;
	std::cout << "Day15 Part2: " << max << std::endl;
}
