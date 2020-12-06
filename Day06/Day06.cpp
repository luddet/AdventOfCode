#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <vector>
#include <algorithm>

std::set<char> intersectSets(const std::vector<std::set<char>>& sets)
{
    std::set<char> result;

    if (sets.empty())
        return result;

    if (sets.size() == 1)
        return sets[0];

    for (auto c : sets[0])
        if (std::all_of(std::begin(sets) + 1, std::end(sets), [c](const std::set<char> otherSet) { return otherSet.find(c) != otherSet.end(); }))
            result.insert(c);

    return result;
}

int main()
{
    std::ifstream ifs("input.txt");
    std::string line;
    uint32_t part1Count(0);
    uint32_t part2Count(0);
    std::set<char> groupAnswers;
    std::vector<std::set<char>> individualAnswers;
    while (getline(ifs, line))
    {
        if (line.empty()) // Finish up group
        {
            part1Count += groupAnswers.size();
            groupAnswers.clear();

            part2Count += intersectSets(individualAnswers).size();
            individualAnswers.clear();
            continue;
        }

        std::set<char> s;
        for (auto c : line)
        {
            groupAnswers.insert(c);
            s.insert(c);
        }
        individualAnswers.push_back(s);
    }

    part1Count += groupAnswers.size();
    part2Count += intersectSets(individualAnswers).size();

    std::cout << "Day06 Part 1: " << int(part1Count) << std::endl;
    std::cout << "Day06 Part 2: " << int(part2Count) << std::endl;
}
