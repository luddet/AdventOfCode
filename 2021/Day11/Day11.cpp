#include <fstream>
#include <iostream>
#include <sstream>
#include <string_view>

#include "../Utilities/grid.h"

using namespace std::string_view_literals;

constexpr const std::string_view test_input{
R"(5483143223
2745854711
5264556173
6141336146
6357385478
4167524645
2176841721
6882881134
4846848554
5283751526)"
};

constexpr const std::string_view input{
R"(8548335644
6576521782
1223677762
1284713113
6125654778
6435726842
5664175556
1445736556
2248473568
6451473526)"
};


int main()
{
	//auto grid = readGrid<char, 10, 10>(test_input);
	//grid.at(1, 2) = '5';
	//std::cout << "Day11 Part 1: " << grid.at(1,2) << '\n';
	//std::cout << "Day11 Part 2: " << '\n';

	constexpr const auto r = extract<char>("aasag"sv);
}
