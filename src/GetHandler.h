#pragma once

#include <string>
#include <vector>

#include "EventHandler.h"

class Telemetry;

class GetHandler : public EventHandler
{
public:
    explicit GetHandler(Telemetry& telemetry);

    void processEvent(const httplib::Request& req,
                      httplib::Response& res) override;

private:
    std::pair<bool, std::string> getResultUnit(const httplib::Request& req,
                                               httplib::Response& res) const;

    static constexpr int SECONDS_INDEX{0};
    static constexpr int MILISECONDS_INDEX{SECONDS_INDEX + 1};

    const std::vector<std::string> validTimeUnitNames_{"seconds",
                                                       "milliseconds"};
};