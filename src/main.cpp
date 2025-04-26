#include <httplib.h>
#include <cassert>
#include <chrono>
#include <cstdlib>
#include <future>
#include <iostream>
#include <nlohmann/json.hpp>
#include <thread>

#include "GetHandler.h"
#include "PostHandler.h"
#include "Telemetry.h"
// #include "Tests.h"

namespace
{
httplib::Params createParams()
{
    httplib::Params params;
    params.emplace("resultUnit", "seconds");  // miliseconds
    params.emplace("startTimestamp", "10");
    params.emplace("endTimestamp", "20");
    return params;
}

std::string createPayload(int date, const std::vector<int>& values)
{
    nlohmann::json payload = nlohmann::json::object();
    payload["date"] = date;
    payload["values"] = values;
    return payload.dump();
}

void testGetting(httplib::Client& client, const std::string& event,
                 int expectedMean)
{
    std::cout << "Client: GET sending" << std::endl;
    httplib::Params params{createParams()};
    auto res = client.Get("/paths/" + event + "/mean", params, {});
    std::cout << "Client: GET reply code=" << res->status
              << " body: " << res->body << std::endl;
    assert(res->status == httplib::StatusCode::OK_200);
    auto jsonResponse = nlohmann::json::parse(res->body);
    // std::string expectedBody
    assert(jsonResponse["mean"] == expectedMean);
}

void testPosting(httplib::Client& client)
{
    std::string jsonType{"application/json"};
    // std::cout << "Client: POST sending" << std::endl;
    // nlohmann::json payload = createPayloadForPostCall();
    // std::cout << "Client: body: " << payload.dump() << std::endl;
    auto res =
        client.Post("/paths/start", createPayload(1, {3, 45, 78, 3}), jsonType);
    assert(res->status == httplib::StatusCode::OK_200);
    std::cout << "Client: POST Reply code=" << res->status
              << " body: " << res->body << std::endl;
    testGetting(client, "start", 32);

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    res = client.Post("/paths/start", createPayload(2, {42, 34, 12}), jsonType);
    assert(res->status == httplib::StatusCode::OK_200);
    std::cout << "Client: POST Reply code=" << res->status
              << " body: " << res->body << std::endl;
    testGetting(client, "start", 31);

    // std::this_thread::sleep_for(std::chrono::milliseconds(100));

    res = client.Post("/paths/start", createPayload(3, {13, 123, 54, 31, 9}),
                      jsonType);
    assert(res->status == httplib::StatusCode::OK_200);
    std::cout << "Client: POST Reply code=" << res->status
              << " body: " << res->body << std::endl;
    testGetting(client, "start", 37);

    res =
        client.Post("/paths/end", createPayload(1, {10, 20, 10, 20}), jsonType);
    assert(res->status == httplib::StatusCode::OK_200);
    testGetting(client, "end", 15);
}

auto getTestingClient(const std::string& address, int port)
{
    return [port, address]()
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        httplib::Client client(address, port);

        testPosting(client);

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        // testGetting(client);
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
    Telemetry telemetry;

    GetHandler getHandler(telemetry);
    svr.Get("/paths/:event/mean",
            [&handler = getHandler](const Request& req, Response& res)
            { handler.processEvent(req, res); });

    PostHandler postHandler(telemetry);
    svr.Post("/paths/:event",
             [&handler = postHandler](const Request& req, Response& res)
             { handler.processEvent(req, res); });

    svr.listen(address, port);

    return EXIT_SUCCESS;

    // clang-format off
    // curl -X GET --json ''  "localhost:8080/paths/start/mean?resultUnit=seconds&startTimestamp=11&endTimestamp=13"
    // curl -X POST --json '{"date":1713897600,"values":[1,2,3,4]}' localhost:8080/paths/start
    // clang-format on
}
