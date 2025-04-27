#include "EventHandler.h"

#include <httplib.h>

EventHandler::EventHandler(Telemetry& telemetry) : telemetry_(telemetry) {}

bool EventHandler::isValidEventName(const std::string& event)
{
    return !event.empty() &&
           std::all_of(event.cbegin(), event.cend(),
                       [](unsigned char c) { return std::isalnum(c); });
}
void EventHandler::raiseError(httplib::Response& res,
                              const std::string& errorMessage)
{
    std::cerr << errorMessage << std::endl;
    res.status = httplib::StatusCode::BadRequest_400;
}
