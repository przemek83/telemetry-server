#include "GetHandler.h"

#include <httplib.h>
#include <charconv>
#include <nlohmann/json.hpp>

#include "Telemetry.h"

GetHandler::GetHandler(Telemetry& telemetry) : EventHandler(telemetry) {}

void GetHandler::processEvent(const httplib::Request& req,
                              httplib::Response& res)
{
    std::string event{req.path_params.at("event")};
    std::cout << "SERVER: " << req.path << " " << event << " " << req.body
              << std::endl;

    int startDate{Telemetry::DATE_NOT_SET};
    int endDate{Telemetry::DATE_NOT_SET};

    if (req.has_param("resultUnit"))
    {
        std::cout << "SERVER: resultUnit: " << req.get_param_value("resultUnit")
                  << std::endl;
    }

    if (req.has_param("startTimestamp"))
    {
        std::string value{req.get_param_value("startTimestamp")};
        std::string_view valueView{value};
        auto [_, errorCode]{
            std::from_chars(valueView.begin(), valueView.end(), startDate)};
        if (errorCode == std::errc())
            std::cout << "SERVER: startDate: " << startDate << std::endl;
        else
            std::cout << "SERVER: startDate: " << value << " conversion error: "
                      << std::make_error_code(errorCode).message() << std::endl;
        // std::cout << "SERVER: startTimestamp: "
        //           << req.get_param_value("startTimestamp") << std::endl;
    }
    if (req.has_param("endTimestamp"))
    {
        std::string value{req.get_param_value("endTimestamp")};
        std::string_view valueView{value};
        auto [_, errorCode]{
            std::from_chars(valueView.begin(), valueView.end(), endDate)};
        if (errorCode == std::errc())
            std::cout << "SERVER: endDate: " << endDate << std::endl;
        else
            std::cout << "SERVER: endDate: " << value << " conversion error: "
                      << std::make_error_code(errorCode).message() << std::endl;
    }
    // std::string key = req.get_param_value("key");
    // std::string count = req.get_param_value("count");
    // std::cout << "SERVER: Key: " << key << " Count: " << count
    //           << std::endl;

    nlohmann::json body;
    int value{telemetry_.computeMean(event, startDate, endDate)};
    body["mean"] = value;
    // std::cout << "SERVER: value: " << value << std::endl;
    res.set_content(body.dump(), "application/json");
    res.status = httplib::StatusCode::OK_200;
}