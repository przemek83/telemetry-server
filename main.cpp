#include <httplib.h>
#include <future>
#include <iostream>
#include <nlohmann/json.hpp>
#include <thread>

namespace
{
httplib::Params createParamsForGetCall()
{
    httplib::Params params;
    params.emplace("resultUnit", "seconds");  // miliseconds
    params.emplace("startTimestamp", "10");
    params.emplace("endTimestamp", "20");
    return params;
}

nlohmann::json createPayloadForPostCall()
{
    nlohmann::json payload;
    payload["values"] = {1, 2, 3, 4};
    payload["date"] = 1713897600;
    return payload;
}

auto getTestingClient()
{
    return []()
    {
        httplib::Client client("0.0.0.0", 8080);
        for (int i{0}; i < 1; ++i)
        {
            std::cout << "Client: GET sending" << std::endl;
            httplib::Params params{createParamsForGetCall()};
            auto res = client.Get("/paths/start/mean", params, {});
            std::cout << "Client: GET reply code=" << res->status
                      << " body: " << res->body << std::endl;

            std::this_thread::sleep_for(std::chrono::seconds(1));

            std::cout << "Client: POST sending" << std::endl;
            nlohmann::json payload{createPayloadForPostCall()};
            res =
                client.Post("/paths/start", payload.dump(), "application/json");
            std::cout << "Client: POST Reply code=" << res->status
                      << " body: " << res->body << std::endl;
        }
    };
}
}  // namespace

int main()
{
    std::cout << "Starting..." << std::endl;

    httplib::Server svr;

    svr.Get("/paths/:event/mean",
            [](const httplib::Request& req, httplib::Response& res)
            {
                std::string event{req.path_params.at("event")};
                std::cout << "SERVER: " << req.path << " " << event << " "
                          << req.body << std::endl;

                if (req.has_param("resultUnit"))
                {
                    std::cout << "SERVER: resultUnit: "
                              << req.get_param_value("resultUnit") << std::endl;
                }
                if (req.has_param("startTimestamp"))
                {
                    std::cout << "SERVER: startTimestamp: "
                              << req.get_param_value("startTimestamp")
                              << std::endl;
                }
                if (req.has_param("endTimestamp"))
                {
                    std::cout << "SERVER: endTimestamp: "
                              << req.get_param_value("endTimestamp")
                              << std::endl;
                }
                // std::string key = req.get_param_value("key");
                // std::string count = req.get_param_value("count");
                // std::cout << "SERVER: Key: " << key << " Count: " << count
                //           << std::endl;

                nlohmann::json body;
                body["mean"] = 34;
                res.set_content(body.dump(4), "application/json");
            });

    svr.Post("/paths/:event",
             [](const httplib::Request& req, httplib::Response& res)
             {
                 std::string event{req.path_params.at("event")};
                 std::cout << "SERVER: " << req.path << " " << event << " "
                           << req.body << std::endl;

                 nlohmann::json parsedData = nlohmann::json::parse(req.body);
                 std::cout << "Values: ";
                 for (int num : parsedData["values"])
                 {
                     std::cout << num << " ";
                 }
                 std::cout << "\nDate: " << parsedData["date"] << std::endl;

                 res.status = httplib::StatusCode::OK_200;
                 res.set_content("{}", "application/json");
             });

    auto server{std::async([&svr]() { svr.listen("0.0.0.0", 8080); })};

    auto testClient{std::async(std::launch::async, getTestingClient())};

    // clang-format off
    // curl -X GET --json ''  "localhost:8080/paths/start/mean?resultUnit=seconds&startTimestamp=11&endTimestamp=13"
    // curl -X POST --json '{"date":1713897600,"values":[1,2,3,4]}' localhost:8080/paths/start
    // clang-format on

    return 0;
}