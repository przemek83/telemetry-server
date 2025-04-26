#include "PostHandler.h"

#include <nlohmann/json.hpp>

#include "Telemetry.h"

PostHandler::PostHandler(Telemetry& telemetry) : telemetry_(telemetry) {}

void PostHandler::processEvent(const httplib::Request& req,
                               httplib::Response& res)
{
    std::string event{req.path_params.at("event")};
    std::cout << "SERVER: " << req.path << " " << event << " " << req.body
              << std::endl;

    nlohmann::json parsedData = nlohmann::json::parse(req.body);

    std::vector<int> values = parsedData["values"].get<std::vector<int>>();

    std::cout << "Values: ";
    for (int num : values)
        std::cout << num << " ";

    std::cout << "\n";
    int date = parsedData["date"];
    std::cout << "Date: " << date << std::endl;

    telemetry_.addEntry(event, date, values);

    res.status = httplib::StatusCode::OK_200;
    res.set_content("{}", "application/json");
}