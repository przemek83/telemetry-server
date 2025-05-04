#include "GetHandler.h"

#include <httplib.h>
#include <algorithm>
#include <charconv>
#include <nlohmann/json.hpp>
#include <string>

#include "Logger.h"
#include "Telemetry.h"

void GetHandler::processEvent(const httplib::Request& req,
                              httplib::Response& res)
{
    logger_.info("start GET " + req.path);

    std::string event{req.path_params.at("event")};

    if (!isValidEventName(event))
    {
        raiseError(res, "Invalid event name: " + event);
        return;
    }

    const auto [success, params]{parseRequest(req, res)};
    if (!success)
        return;

    const auto& [resultUnit, startDate, endDate]{params};

    int mean{telemetry_.computeMean(event, startDate, endDate)};
    if (resultUnit == timeUnitNames_[MILISECONDS_INDEX])
        mean *= MILLISECONDS_IN_SECOND;

    fillResponse(res, mean);

    logger_.info("end GET " + req.path +
                 ": computed mean = " + std::to_string(mean));
}

std::pair<bool, GetHandler::RequestParams> GetHandler::parseRequest(
    const httplib::Request& req, httplib::Response& res)
{
    const auto [success, resultUnit]{getResultUnit(req, res)};
    if (!success)
        return {false, {}};

    const auto [successStartDate,
                startDate]{getDate(req, "startTimestamp", res)};
    if (!successStartDate)
        return {false, {}};

    const auto [successEndDate, endDate]{getDate(req, "endTimestamp", res)};
    if (!successEndDate)
        return {false, {}};

    return {true, {resultUnit, startDate, endDate}};
}

std::pair<bool, std::string> GetHandler::getResultUnit(
    const httplib::Request& req, httplib::Response& res)
{
    if (!req.has_param("resultUnit"))
    {
        raiseError(res, "Parameter 'resultUnit' is missing");
        return {false, ""};
    }

    std::string resultUnit{req.get_param_value("resultUnit")};
    if (std::find(timeUnitNames_.cbegin(), timeUnitNames_.cend(), resultUnit) ==
        timeUnitNames_.cend())
    {
        std::string validTimeUnitNames;
        for (const auto& unit : timeUnitNames_)
            validTimeUnitNames += unit + ",";
        validTimeUnitNames.pop_back();

        const std::string errorMessage{
            "Invalid 'resultUnit' value: " + resultUnit +
            ". Valid values are: " + validTimeUnitNames};

        raiseError(res, errorMessage);
        return {false, ""};
    }

    return {true, resultUnit};
}

std::pair<bool, int> GetHandler::getDate(const httplib::Request& req,
                                         const std::string& paramName,
                                         httplib::Response& res)
{
    int date{Telemetry::DATE_NOT_SET};

    if (!req.has_param(paramName))
        return {true, date};

    const std::string value{req.get_param_value(paramName)};
    const auto [mismatch, errorCode]{
        std::from_chars(value.data(), value.data() + value.size(), date)};

    if (errorCode != std::errc() || mismatch != (value.data() + value.size()))
    {
        std::string errorMessage{"Invalid value for '" + paramName +
                                 "': " + value + ". Expected an integer."};
        raiseError(res, errorMessage);
        return {false, date};
    }

    return {true, date};
}

void GetHandler::fillResponse(httplib::Response& res, int mean)
{
    nlohmann::json body;
    body["mean"] = mean;

    res.set_content(body.dump(), "application/json");
    res.status = httplib::StatusCode::OK_200;
}