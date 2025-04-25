#include "PostHandler.h"

#include <nlohmann/json.hpp>

void PostHandler::processEvent(const httplib::Request& req,
                               httplib::Response& res)
{
    std::string event{req.path_params.at("event")};
    std::cout << "SERVER: " << req.path << " " << event << " " << req.body
              << std::endl;

    nlohmann::json parsedData = nlohmann::json::parse(req.body);
    std::cout << "Values: ";
    for (int num : parsedData["values"])
    {
        std::cout << num << " ";
    }
    std::cout << "\nDate: " << parsedData["date"] << std::endl;

    res.status = httplib::StatusCode::OK_200;
    res.set_content("{}", "application/json");
}