#include "PostHandler.h"

#include <algorithm>
#include <nlohmann/json.hpp>

#include "Telemetry.h"

PostHandler::PostHandler(Telemetry& telemetry) : telemetry_(telemetry) {}

void PostHandler::processEvent(const httplib::Request& req,
                               httplib::Response& res)
{
    std::string event{req.path_params.at("event")};
    std::cout << "SERVER: " << req.path << " " << event << " " << req.body
              << std::endl;

    if (event.empty() ||
        !std::all_of(event.begin(), event.end(),
                     [](unsigned char c) { return std::isalnum(c); }))
    {
        std::cerr << "Invalid event name: " << event << std::endl;
        res.status = httplib::StatusCode::BadRequest_400;
        res.set_content("{}", "application/json");
        return;
    }

    nlohmann::json parsedData;
    try
    {
        parsedData = nlohmann::json::parse(req.body);
    }
    catch (const nlohmann::json::parse_error& ex)
    {
        std::cerr << "Parse error at byte " << ex.byte << ": " << ex.what()
                  << std::endl;
        res.status = httplib::StatusCode::BadRequest_400;
        res.set_content("{}", "application/json");
        return;
    }
    if (!parsedData.contains("values") || !parsedData.contains("date"))
    {
        std::cerr << "Missing required fields in JSON payload." << std::endl;
        res.status = httplib::StatusCode::BadRequest_400;
        res.set_content("{}", "application/json");
        return;
    }

    if (!parsedData["values"].is_array())
    {
        std::cerr << "Values is not an array." << std::endl;
        res.status = httplib::StatusCode::BadRequest_400;
        res.set_content("{}", "application/json");
        return;
    }

    std::vector<int> values = parsedData["values"].get<std::vector<int>>();
    if (values.empty())
    {
        std::cerr << "Values array is empty." << std::endl;
        res.status = httplib::StatusCode::BadRequest_400;
        res.set_content("{}", "application/json");
        return;
    }

    std::cout << "Values: ";
    for (int num : values)
        std::cout << num << " ";

    std::cout << "\n";
    if (parsedData["date"].is_string())
    {
        std::cerr << "Date is a string, expected an integer." << std::endl;
        res.status = httplib::StatusCode::BadRequest_400;
        res.set_content("{}", "application/json");
        return;
    }
    if (!parsedData["date"].is_number_integer())
    {
        std::cerr << "Date is not an integer." << std::endl;
        res.status = httplib::StatusCode::BadRequest_400;
        res.set_content("{}", "application/json");
        return;
    }
    int date = parsedData["date"];
    std::cout << "Date: " << date << std::endl;

    telemetry_.addEntry(event, date, values);

    res.status = httplib::StatusCode::OK_200;
    res.set_content("{}", "application/json");
}