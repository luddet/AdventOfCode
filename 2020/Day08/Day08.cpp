#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <tuple>

std::string EXAMPLE_PROGRAM(
    "nop +0\n"
    "acc +1\n"
    "jmp +4\n"
    "acc +3\n"
    "jmp -3\n"
    "acc -99\n"
    "acc +1\n"
    "jmp -4\n"
    "acc +6"
);

using program_t = std::vector<std::tuple<std::string, int32_t>>;

program_t parseInput(std::istream& is)
{
    program_t program;
    std::string line;
    while (std::getline(is, line))
    {
        program.push_back(std::make_tuple(line.substr(0,3), std::stoi(line.substr(4))));
    }
    return program;
}

// return true if terminated normally without infinite loop
bool runProgram(program_t& program, int32_t& acc)
{
    size_t ip(0);
    std::set<size_t> visitedInstructions;
    std::set<size_t>::const_iterator it;

    while (ip < program.size() && (it = visitedInstructions.find(ip)) == visitedInstructions.end())
    {
        auto& [op, param] = program[ip];
        visitedInstructions.insert(ip);

        if (op == "acc")
        {
            acc += param;
        }
        else if (op == "jmp")
        {
            ip += param;
            continue;
        }
        ++ip;
    }
    return it == visitedInstructions.end();
}

int main()
{
    //std::istringstream is(EXAMPLE_PROGRAM);
    std::ifstream is("input.txt");

    program_t program(parseInput(is));

    int32_t acc(0);
    runProgram(program, acc);
    int32_t part1 = acc;

    for (size_t i = 0; i < program.size(); ++i)
    {
        auto& [op, param] = program[i];
        if (op == "jmp")
        {
            op = "nop";
            acc = 0;
            if (runProgram(program, acc))
                break;
            op = "jmp";
        }
    }
    int32_t part2 = acc;

    std::cout << "Day08 part 1: " << part1 << std::endl;
    std::cout << "Day08 part 2: " << part2 << std::endl;
}
