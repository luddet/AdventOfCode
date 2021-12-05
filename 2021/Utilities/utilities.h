#pragma once
#include <vector>
#include <string>
#include <istream>

#include "dllexport.h"

DLLEXPORT std::vector<int> readInts(std::istream& os);
DLLEXPORT std::vector<std::string> readLines(const std::string& filePath);
