#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <cassert>
#include <algorithm>
#include <functional>
#include <regex>

using passport_t = std::map<std::string, std::string>;

bool parsePassport(std::istream& is, passport_t& passport)
{
    std::string line;
    is >> std::ws;
    while (std::getline(is, line) && !line.empty())
    {
        std::istringstream lineStream(line);
        std::string property;
        while (std::getline(lineStream, property, ' '))
        {
            assert(line[3] == ':');
            std::string propName = property.substr(0, 3);
            std::string propValue = property.substr(4);
            passport[propName] = propValue;
        }
    }
    return passport.size() > 0;
}

bool hasRequiredProperties(passport_t& passport)
{
    std::vector<std::string> requiredProperties
    {
        "byr",
        "iyr",
        "eyr",
        "hgt",
        "hcl",
        "ecl",
        "pid",
    };

    return std::all_of(std::begin(requiredProperties), std::end(requiredProperties),
        [&passport](auto& str){ return passport.find(str) != passport.end(); });
}

bool isInRange(std::string str, int min, int max)
{
    int n = std::stoi(str);
    return n >= min && n <= max;
}

using validators_t = std::map<const std::string, std::function<bool(const std::string&)>>;
validators_t validators
{
    //byr (Birth Year) - four digits; at least 1920 and at most 2002.
    { "byr", [](const std::string& str) { return isInRange(str, 1920, 2002); } },
    
    //iyr (Issue Year) - four digits; at least 2010 and at most 2020.
    { "iyr", [](const std::string& str) { return isInRange(str, 2010, 2020); } },

    //eyr (Expiration Year) - four digits; at least 2020 and at most 2030.
    { "eyr", [](const std::string& str) { return isInRange(str, 2020, 2030); } },

    //hgt (Height) - a number followed by either cm or in:
    //If cm, the number must be at least 150 and at most 193.
    //If in, the number must be at least 59 and at most 76.
    { "hgt", [](const std::string& str) {
        if (str.length() < 3) 
            return false;
        else if (str.substr(str.length() - 2) == "in")
        {
            int n = std::stoi(str.substr(0, str.length() - 2));
            return n >= 59 && n <= 76;
        }
        else if (str.substr(str.length() - 2) == "cm")
        {
            int n = std::stoi(str.substr(0, str.length() - 2));
            return n >= 150 && n <= 193;
        }
        else
            return false;
    }},

    //hcl (Hair Color) - a # followed by exactly six characters 0-9 or a-f.
    { "hcl", [](const std::string& str) { return std::regex_match(str, std::regex("#[0-9a-f]{6}")); } },

    //ecl (Eye Color) - exactly one of: amb blu brn gry grn hzl oth.
    { "ecl", [](const std::string& str) { return std::regex_match(str, std::regex("(amb|blu|brn|gry|grn|hzl|oth)")); } },

    //pid (Passport ID) - a nine-digit number, including leading zeroes.
    { "pid", [](const std::string& str) { return std::regex_match(str, std::regex("\\d{9}")); } },
};

bool propertiesAreValid(passport_t passport, validators_t validators)
{
    for (auto& v : validators)
    {
        auto predicate = v.second;
        auto key = v.first;
        auto property_it = passport.find(key);
        if (property_it == passport.end() || !predicate( (*property_it).second))
            return false;
    }
    return true;
}

int main()
{
    std::ifstream input("input.txt");
    
    uint16_t validPassports1 = 0;
    uint16_t validPassports2 = 0;
    while (!input.eof())
    {
        passport_t passport;
        if (parsePassport(input, passport))
        {
            if (hasRequiredProperties(passport))
                ++validPassports1;
            if (propertiesAreValid(passport, validators))
                ++validPassports2;
        }
    }

    std::cout << "Day04 Part 1: " << validPassports1 << std::endl;
    std::cout << "Day04 Part 2: " << validPassports2 << std::endl;
}
