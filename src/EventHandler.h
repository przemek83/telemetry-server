#pragma once

#include <string>

class Telemetry;
namespace httplib
{
struct Request;
struct Response;
}  // namespace httplib

class EventHandler
{
public:
    explicit EventHandler(Telemetry& telemetry);
    virtual ~EventHandler() = default;

    virtual void processEvent(const httplib::Request& req,
                              httplib::Response& res) = 0;

protected:
    static bool isValidEventName(const std::string& event);

    static void raiseError(httplib::Response& res,
                           const std::string& errorMessage);

    Telemetry& telemetry_;
};