#pragma once

#include <string>

class Telemetry;
class Logger;
namespace httplib
{
struct Request;
struct Response;
}  // namespace httplib

class EventHandler
{
public:
    EventHandler(Telemetry& telemetry, Logger& logger);

    virtual ~EventHandler() = default;

    virtual void processEvent(const httplib::Request& req,
                              httplib::Response& res) = 0;

protected:
    static bool isValidEventName(const std::string& event);

    void raiseError(httplib::Response& res, const std::string& errorMessage);

    Telemetry& telemetry_;
    Logger& logger_;
};
