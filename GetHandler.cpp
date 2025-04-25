#include "GetHandler.h"

#include <nlohmann/json.hpp>

void GetHandler::processEvent(const httplib::Request& req,
                              httplib::Response& res)
{
    std::string event{req.path_params.at("event")};
    std::cout << "SERVER: " << req.path << " " << event << " " << req.body
              << std::endl;

    if (req.has_param("resultUnit"))
    {
        std::cout << "SERVER: resultUnit: " << req.get_param_value("resultUnit")
                  << std::endl;
    }
    if (req.has_param("startTimestamp"))
    {
        std::cout << "SERVER: startTimestamp: "
                  << req.get_param_value("startTimestamp") << std::endl;
    }
    if (req.has_param("endTimestamp"))
    {
        std::cout << "SERVER: endTimestamp: "
                  << req.get_param_value("endTimestamp") << std::endl;
    }
    // std::string key = req.get_param_value("key");
    // std::string count = req.get_param_value("count");
    // std::cout << "SERVER: Key: " << key << " Count: " << count
    //           << std::endl;

    nlohmann::json body;
    body["mean"] = 34;
    res.set_content(body.dump(4), "application/json");
}