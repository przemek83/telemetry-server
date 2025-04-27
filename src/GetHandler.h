#pragma once

#include <string>
#include <vector>

#include "EventHandler.h"
#include "Telemetry.h"

class GetHandler : public EventHandler
{
public:
    using EventHandler::EventHandler;

    void processEvent(const httplib::Request& req,
                      httplib::Response& res) override;

private:
    struct RequestParams
    {
        std::string telemetryUnit;
        int startDate{Telemetry::DATE_NOT_SET};
        int endDate{Telemetry::DATE_NOT_SET};
    };

    std::pair<bool, RequestParams> parseRequest(const httplib::Request& req,
                                                httplib::Response& res);

    std::pair<bool, std::string> getResultUnit(const httplib::Request& req,
                                               httplib::Response& res);

    std::pair<bool, int> getDate(const httplib::Request& req,
                                 const std::string& paramName,
                                 httplib::Response& res);

    static void fillResponse(httplib::Response& res, int mean);

    static constexpr int MILLISECONDS_IN_SECOND{1000};
    static constexpr int SECONDS_INDEX{0};
    static constexpr int MILISECONDS_INDEX{SECONDS_INDEX + 1};

    const std::vector<std::string> validTimeUnitNames_{"seconds",
                                                       "milliseconds"};
};