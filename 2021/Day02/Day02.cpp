#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

#include "../Utilities/utilities.h"

int part1(std::vector<std::tuple<std::string, int>>& instructions)
{
    int pos(0), depth(0);
    for (auto& [instr, num] : instructions)
    {
        if (instr == "forward")
            pos += num;
        else if (instr == "up")
            depth -= num;
        else if (instr == "down")
            depth += num;
    }
    return depth * pos;
}

int part2(std::vector<std::tuple<std::string, int>>& instructions)
{
    int aim(0), depth(0), pos(0);
    for (auto& [instr, num] : instructions)
    {
        if (instr == "forward")
        {
            pos += num;
            depth += aim * num;
        }
        else if (instr == "up")
            aim -= num;
        else if (instr == "down")
            aim += num;
    }
    return pos * depth;;
}

int main()
{
    using std::vector;
    using std::string;
    using std::tuple;

    vector<string> lines(readLines("input.txt"));

    vector<tuple<string, int>> instructions(lines.size());
    std::for_each(lines.begin(), lines.end(), [&instructions](auto str)
    {
        std::istringstream is(str);
        std::string instr;
        int num;
        is >> instr >> num;
        instructions.push_back({ instr, num });
    });

    std::cout << "Day02 part 1: " << part1(instructions) << std::endl;
    std::cout << "Day02 part 2: " << part2(instructions) << std::endl;

}
