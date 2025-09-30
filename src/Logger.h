#include <string>

class Logger
{
public:
    virtual ~Logger() = default;

    virtual void info(const std::string& message) = 0;
    virtual void error(const std::string& message) = 0;
};
