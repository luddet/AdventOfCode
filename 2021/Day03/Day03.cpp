#include <iostream>
#include <vector>
#include <cassert>

#include "../Utilities/utilities.h"

int main()
{
    auto lines(readLines("input.txt"));
    constexpr size_t NUM_BITS(12);
    uint32_t numOnes[12]{0,0,0,0,0,0,0,0,0,0,0,0};

    for (auto& line : lines)
    {
        assert(line.length() == NUM_BITS);
        for (size_t i = 0; i < line.length(); ++i)
        {
            if (line[i] == '1')
                ++numOnes[i];
        }
    }

    uint16_t gamma(0);
    size_t numLines(lines.size());
    for (size_t i = 0; i < NUM_BITS; ++i)
    {
        if (numOnes[i] > numLines - numOnes[i])
            gamma |= 1 << NUM_BITS - i - 1;
    }

    uint16_t epsilon = ~gamma & 0b0000111111111111;


    std::cout << "Day03 part 1: " << (epsilon * gamma) << std::endl;
}
