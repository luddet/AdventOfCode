// https://adventofcode.com/2019/day/9

#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#include "Computer.h"

Computer::mem_container_t parseInput(std::istream& stream)
{
    Computer::mem_container_t memory;
    Computer::mem_item_t n;
    while (stream >> n)
    {
        memory.push_back(n);
        char comma; stream >> comma;
    }
    return memory;
}

int main()
{
    std::fstream fs("input.txt");
    Computer::mem_container_t originalMemory(parseInput(fs));

    Computer computer(originalMemory);
    std::cout << "Running tests...";
    computer.addInput(1); // test mode
    computer.run();
    assert(computer.isDone());
    std::cout << " OK" << std::endl;

    Computer::mem_item_t outputPart1;
    while (computer.getOutput(outputPart1))
        std::cout << "Part 1: " << outputPart1 << std::endl;

    Computer computer2(originalMemory);
    computer2.addInput(2); // boost mode
    std::cout << "Running boost...";
    computer2.run();
    assert(computer2.isDone());
    std::cout << " OK." << std::endl;
    Computer::mem_item_t outputPart2;
    while (computer2.getOutput(outputPart2))
        std::cout << "Part 2: " << outputPart2 << std::endl;
}
