#pragma once

#include <src/Logger.h>

class FakeLogger : public Logger
{
public:
    void info([[maybe_unused]] const std::string& message) override
    {
        // skip logging
    }

    void error([[maybe_unused]] const std::string& message) override
    {
        // skip logging}
    }
};
