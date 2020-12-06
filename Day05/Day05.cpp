#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <limits>
#include <algorithm>



uint16_t decodeStr(const std::string& str)
{
    size_t length = str.length();
    uint16_t row = 0;
    for (size_t i = 0; i < length; ++i)
    {
        if (str[i] == 'B' || str[i] == 'R')
            row |= 1 << (length - 1 - i);
    }
    return row;
}

uint16_t getRow(const std::string& str)
{
    return decodeStr(str.substr(0,7));
}

uint16_t getCol(const std::string& str)
{
    return decodeStr(str.substr(7));
}

uint16_t getSeatId(const std::string& str)
{
    return getRow(str) * 8 + getCol(str);
}

int main()
{
    std::ifstream ifs("input.txt");
    std::vector<uint16_t> ids;
    std::string line;
    while (std::getline(ifs, line))
        ids.push_back(getSeatId(line));
        
    std::sort(begin(ids), end(ids));

    uint16_t myId = 0;
    for (size_t i = 0; i < ids.size() - 1 && myId == 0; ++i)
        if (ids[i] + 1 != ids[i + 1])
            myId = ids[i] + 1;

    std::cout << "Day05 Part 1: " << ids[ids.size() - 1] << std::endl;
    std::cout << "Day05 Part 2: " << myId << std::endl;

}

