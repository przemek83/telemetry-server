#pragma once

#include "nlohmann/json_fwd.hpp"

#include "EventHandler.h"

class Telemetry;

class PostHandler : public EventHandler
{
public:
    using EventHandler::EventHandler;

    void processEvent(const httplib::Request& req,
                      httplib::Response& res) override;

private:
    std::tuple<bool, int, std::vector<int>> parsePayload(
        const std::string& payload, httplib::Response& res);

    std::tuple<bool, int, std::vector<int>> parseData(
        const nlohmann::json& parsedData, httplib::Response& res);

    std::pair<bool, std::vector<int>> parseValues(
        const nlohmann::json& parsedData, httplib::Response& res);

    std::pair<bool, int> parseDate(const nlohmann::json& parsedData,
                                   httplib::Response& res);
};
