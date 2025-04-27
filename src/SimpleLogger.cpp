#include "SimpleLogger.h"

#include <iostream>
#include <sstream>

void SimpleLogger::info(const std::string& message)
{
    std::stringstream stream;
    stream << "[INFO]: " << message << "\n";
    std::cout << stream.str();
}

void SimpleLogger::error(const std::string& message)
{
    std::stringstream stream;
    stream << "[ERRO]: " << message << "\n";
    std::cout << stream.str();
}