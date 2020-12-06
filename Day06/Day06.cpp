#include <iostream>
#include <fstream>
#include <string>
#include <set>

int main()
{
    std::ifstream ifs("input.txt");
    std::string line;
    uint32_t totalCount(0);
    std::set<char> groupAnswers;
    while (getline(ifs, line))
    {
        if (line.empty())
        {
            totalCount += groupAnswers.size();
            groupAnswers.clear();
            continue;
        }

        for (auto c : line)
            groupAnswers.insert(c);
    }

    totalCount += groupAnswers.size();

    std::cout << "Day06 Part 1: " << int(totalCount) << std::endl;
}
