#pragma once

#include "Logger.h"

class SimpleLogger : public Logger
{
public:
    void info(const std::string& message) override;
    void error(const std::string& message) override;
};
