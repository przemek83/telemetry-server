#include <httplib.h>
#include <future>
#include <iostream>
#include <nlohmann/json.hpp>
#include <thread>

#include "GetHandler.h"
#include "PostHandler.h"

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
    nlohmann::json payload = nlohmann::json::object();
    payload["date"] = 1713897600;
    payload["values"] = {1, 2, 3, 4};
    return payload;
}

auto getTestingClient(const std::string& address, int port)
{
    return [port, address]()
    {
        httplib::Client client(address, port);
        for (int i{0}; i < 1; ++i)
        {
            std::cout << "Client: GET sending" << std::endl;
            httplib::Params params{createParamsForGetCall()};
            auto res = client.Get("/paths/start/mean", params, {});
            std::cout << "Client: GET reply code=" << res->status
                      << " body: " << res->body << std::endl;

            std::this_thread::sleep_for(std::chrono::seconds(1));

            std::cout << "Client: POST sending" << std::endl;
            nlohmann::json payload = createPayloadForPostCall();
            std::cout << "Client: body: " << payload.dump() << std::endl;
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
    std::cout << "Starting server..." << std::endl;

    std::string address("0.0.0.0");
    const int port{8080};

    using httplib::Request;
    using httplib::Response;

    httplib::Server svr;

    GetHandler getHandler;
    svr.Get("/paths/:event/mean",
            [&handler = getHandler](const Request& req, Response& res)
            { handler.processEvent(req, res); });

    PostHandler postHandler;
    svr.Post("/paths/:event",
             [&handler = postHandler](const Request& req, Response& res)
             { handler.processEvent(req, res); });

    auto server{std::async([&svr, address]() { svr.listen(address, port); })};

    auto testClient{
        std::async(std::launch::async, getTestingClient(address, port))};

    // clang-format off
    // curl -X GET --json ''  "localhost:8080/paths/start/mean?resultUnit=seconds&startTimestamp=11&endTimestamp=13"
    // curl -X POST --json '{"date":1713897600,"values":[1,2,3,4]}' localhost:8080/paths/start
    // clang-format on

    return 0;
}