#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cassert>

uint16_t countTrees(std::vector<std::string>& map, size_t stepX, size_t stepY)
{
    size_t x(0), y(0);
    size_t mapWidth = map[0].length();
    size_t mapHeight = map.size();
    uint16_t treeCount = 0;

    while (y < mapHeight)
    {
        if (map[y][x] == '#')
            ++treeCount;

        x = (x + stepX) % mapWidth;
        y += stepY;
    }
    return treeCount;
}

int main()
{
    std::vector<std::string> testInput
    {
        "..##.......",
        "#...#...#..",
        ".#....#..#.",
        "..#.#...#.#",
        ".#...##..#.",
        "..#.##.....",
        ".#.#.#....#",
        ".#........#",
        "#.##...#...",
        "#...##....#",
        ".#..#...#.#",
    };

    auto testTreeCount = countTrees(testInput, 3, 1);
    assert(testTreeCount == 7);

    std::ifstream ifs("input.txt");
    std::vector<std::string> map;
    std::string line;
    while (std::getline(ifs, line))
        map.push_back(line);

    auto treeCount = countTrees(map, 3, 1);
    uint32_t treeProduct = 1;
    std::vector<std::pair<size_t, size_t>> slopes
    {
        {1, 1},
        {3, 1},
        {5, 1},
        {7, 1},
        {1, 2}
    };

    for (auto& slope : slopes)
        treeProduct *= countTrees(map, slope.first, slope.second);

    std::cout << "Day03 Part 1: " << treeCount << std::endl;
    std::cout << "Day03 Part 2: " << treeProduct << std::endl;

}
