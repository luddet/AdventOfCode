// https://adventofcode.com/2019/day/6

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>
#include <unordered_map>

using orbit_map = std::unordered_map<std::string, std::string>;

orbit_map buildTree(std::vector<std::string>& input)
{
    std::unordered_map<std::string, std::string> objects;
    for (auto line : input)
    {   
        size_t delimIndex = line.find(')');
        assert(delimIndex != std::string::npos);
        objects.insert(std::make_pair(line.substr(delimIndex + 1), line.substr(0, delimIndex)));
    }

    return objects;
}

uint32_t orbitCount(const orbit_map& map)
{
    uint32_t count = 0;
    for (auto pair : map)
    {
        count++;
        auto currentPair = map.find(pair.second);
        while (currentPair != map.end())
        {
            count++;
            currentPair = map.find(currentPair->second);
        }
    }
    return count;
}

int main()
{
    std::fstream fs("input.txt");
    std::string line;
    std::vector<std::string> input;
    while (std::getline(fs, line))
        input.push_back(line);

    orbit_map map = buildTree(input);
    uint32_t count = orbitCount(map);

    std::cout << "Part 1: " << count << std::endl;
}
