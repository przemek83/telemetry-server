#pragma once

#include <string>
#include <vector>

#include "EventHandler.h"

class Telemetry;

class GetHandler : public EventHandler
{
public:
    using EventHandler::EventHandler;

    void processEvent(const httplib::Request& req,
                      httplib::Response& res) override;

private:
    std::pair<bool, std::string> getResultUnit(const httplib::Request& req,
                                               httplib::Response& res);

    std::pair<bool, int> getDate(const httplib::Request& req,
                                 const std::string& paramName,
                                 httplib::Response& res);

    static constexpr int SECONDS_INDEX{0};
    static constexpr int MILISECONDS_INDEX{SECONDS_INDEX + 1};

    const std::vector<std::string> validTimeUnitNames_{"seconds",
                                                       "milliseconds"};
};