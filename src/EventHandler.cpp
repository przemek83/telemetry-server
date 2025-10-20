#include "EventHandler.h"

#include <locale>

#include <httplib.h>
#include <string_view>

#include "Logger.h"

EventHandler::EventHandler(Telemetry& telemetry, Logger& logger)
    : telemetry_(telemetry), logger_(logger){};

bool EventHandler::isValidEventName(std::string_view event)
{
    return (!event.empty()) &&
           std::all_of(event.cbegin(), event.cend(),
                       [](char c) { return std::isalnum(c, std::locale{}); });
}
void EventHandler::raiseError(httplib::Response& res,
                              const std::string& errorMessage)
{
    logger_.error(errorMessage);
    res.status = httplib::StatusCode::BadRequest_400;
}
