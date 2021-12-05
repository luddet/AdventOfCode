#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "../Utilities/utilities.h"

std::vector<std::string> readLines(const std::string& filePath)
{
    std::ifstream ifs(filePath);
    std::string line;
    std::vector<std::string> result;
    while (std::getline(ifs, line))
        result.push_back(line);
    return result;
}


int main()
{
    std::vector<std::string> lines(readLines("input.txt"));

    int pos(0), depth(0);
    for (auto& line : lines)
    {
        int num = std::stoi(line.substr(line.find(' '), line.size()));
        if (line.starts_with("forward"))
        {
            pos += num;
        }
        else if (line.starts_with("up"))
        {
            depth -= num;
        }
        else if (line.starts_with("down"))
        {
            depth += num;
        }
    }

    std::cout << "Day02 part 1: " << (depth * pos) << std::endl;

}
