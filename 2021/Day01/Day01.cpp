// https://adventofcode.com/2021/day/1

#include <iostream>
#include <fstream>

int main()
{
    std::ifstream is("input.txt");
    uint32_t lastDepth, currentDepth;
    uint32_t numIncrements(0);

    is >> lastDepth;
    while (!is.eof())
    {
        is >> currentDepth;
        if (currentDepth > lastDepth)
            ++numIncrements;
        lastDepth = currentDepth;
    }
    std::cout << "Day01 part 1: " << numIncrements << std::endl;
}