#include "EventHandler.h"

#include <httplib.h>

#include "Logger.h"

EventHandler::EventHandler(Telemetry& telemetry, Logger& logger)
    : telemetry_(telemetry), logger_(logger){};

bool EventHandler::isValidEventName(const std::string& event)
{
    return (!event.empty()) &&
           std::all_of(event.cbegin(), event.cend(),
                       [](unsigned char c) { return std::isalnum(c); });
}
void EventHandler::raiseError(httplib::Response& res,
                              const std::string& errorMessage)
{
    logger_.error(errorMessage);
    res.status = httplib::StatusCode::BadRequest_400;
}
