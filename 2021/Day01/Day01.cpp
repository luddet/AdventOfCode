// https://adventofcode.com/2021/day/1

#include <iostream>
#include <fstream>
#include <numeric>
#include "utilities.h"

uint32_t part1(const std::vector<int>& values)
{
    uint32_t lastDepth, currentDepth;
    uint32_t numIncrements(0);

    auto it = std::begin(values);
    lastDepth = *(it++);
    while (it != std::end(values))
    {
        currentDepth = *(it++);
        if (currentDepth > lastDepth)
            ++numIncrements;
        lastDepth = currentDepth;
    }
    return numIncrements;
}

uint32_t part2(const std::vector<int>& values)
{
    uint32_t numIncrements(0);
    auto wStart = std::begin(values);
    auto wEnd = std::next(std::begin(values), 3);
    int lastWindow(std::accumulate(wStart, wEnd, 0));
    int currentWindow(lastWindow);

    while (wEnd != std::end(values))
    {
        currentWindow -= *wStart;
        std::advance(wStart, 1);
        currentWindow += *wEnd;
        std::advance(wEnd, 1);

        if (currentWindow > lastWindow)
            ++numIncrements;
        lastWindow = currentWindow;
    }
    return numIncrements;
}

int main()
{
    std::ifstream ifs("input.txt");
    auto values = readInts(ifs);

    std::cout << "Day01 part 1: " << part1(values) << std::endl;
    std::cout << "Day02 part 2: " << part2(values) << std::endl;
}