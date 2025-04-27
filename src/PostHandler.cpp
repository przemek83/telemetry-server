#include "PostHandler.h"

#include <httplib.h>
#include <nlohmann/json.hpp>

#include "Telemetry.h"

PostHandler::PostHandler(Telemetry& telemetry) : EventHandler(telemetry) {}

void PostHandler::processEvent(const httplib::Request& req,
                               httplib::Response& res)
{
    std::string event{req.path_params.at("event")};
    std::cout << "SERVER: " << req.path << " " << event << " " << req.body
              << std::endl;

    res.set_content(nlohmann::json::object().dump(), "application/json");

    if (!isValidEventName(event))
    {
        raiseError(res, "Invalid event name: " + event);
        return;
    }

    const auto [success, date, values]{parsePayload(req.body, res)};
    if (!success)
        return;

    std::cout << "Date: " << date << std::endl;

    telemetry_.addEntry(event, date, values);
    res.status = httplib::StatusCode::OK_200;
}

std::tuple<bool, int, std::vector<int>> PostHandler::parsePayload(
    const std::string& payload, httplib::Response& res)
{
    nlohmann::json parsedData;
    try
    {
        parsedData = nlohmann::json::parse(payload);
    }
    catch (const nlohmann::json::parse_error& ex)
    {
        raiseError(res, "JSON parse error: " + std::string(ex.what()));
        return {false, 0, {}};
    }

    const auto [valuesOK, values]{parseValues(parsedData, res)};
    if (!valuesOK)
        return {false, 0, {}};

    const auto [dateOK, date]{parseDate(parsedData, res)};
    if (!dateOK)
        return {false, 0, {}};

    return {true, date, values};
}

std::pair<bool, std::vector<int>> PostHandler::parseValues(
    const nlohmann::json& parsedData, httplib::Response& res)
{
    if (!parsedData.contains("values"))
    {
        raiseError(res, "Missing required values fields in JSON payload.");
        return {false, {}};
    }

    if (!parsedData["values"].is_array())
    {
        raiseError(res, "Values is not an array.");
        return {false, {}};
    }

    std::vector<int> values{parsedData["values"].get<std::vector<int>>()};

    return {true, values};
}

std::pair<bool, int> PostHandler::parseDate(const nlohmann::json& parsedData,
                                            httplib::Response& res)
{
    if (!parsedData.contains("date"))
    {
        raiseError(res, "Missing required date fields in JSON payload.");
        return {false, 0};
    }

    if (!parsedData["date"].is_number_integer())
    {
        raiseError(res, "Date is not an integer.");
        return {false, 0};
    }

    return {true, parsedData["date"]};
}
