#include <httplib.h>
#include <catch2/catch_test_macros.hpp>
#include <nlohmann/json.hpp>
#include "tests/FakeLogger.h"

#include <src/PostHandler.h>
#include <src/Telemetry.h>

namespace
{
std::string createPayload(int date, const std::vector<int>& values)
{
    nlohmann::json payload = nlohmann::json::object();
    payload["date"] = date;
    payload["values"] = values;
    return payload.dump();
}
}  // namespace

TEST_CASE("PostHandler Tests", "[rest-server]")
{
    httplib::Request request;
    httplib::Response response;

    Telemetry telemetry;
    FakeLogger logger;
    const std::string eventName{"TestEvent"};
    PostHandler postHandler(telemetry, logger);

    SECTION("Posting data and verifying mean")
    {
        request.path_params["event"] = eventName;
        request.body = createPayload(1, {3, 45, 78, 3});
        postHandler.processEvent(request, response);
        REQUIRE(response.status == httplib::StatusCode::OK_200);
        REQUIRE(telemetry.computeMean(eventName, Telemetry::DATE_NOT_SET,
                                      Telemetry::DATE_NOT_SET) == 32);

        request.path_params["event"] = eventName;
        request.body = createPayload(2, {23, 43, 123, 13});
        postHandler.processEvent(request, response);
        REQUIRE(response.status == httplib::StatusCode::OK_200);
        REQUIRE(telemetry.computeMean(eventName, Telemetry::DATE_NOT_SET,
                                      Telemetry::DATE_NOT_SET) == 41);
    }

    SECTION("Invalid payload handling")
    {
        request.path_params["event"] = eventName;
        request.body = "Invalid JSON";
        postHandler.processEvent(request, response);
        REQUIRE(response.status == httplib::StatusCode::BadRequest_400);
    }

    SECTION("Empty payload handling")
    {
        request.path_params["event"] = eventName;
        request.body = "{}";
        postHandler.processEvent(request, response);
        REQUIRE(response.status == httplib::StatusCode::BadRequest_400);
    }

    SECTION("Missing values in payload")
    {
        request.path_params["event"] = eventName;
        request.body = R"({"date": 1})";
        postHandler.processEvent(request, response);
        REQUIRE(response.status == httplib::StatusCode::BadRequest_400);
    }

    SECTION("Missing date in payload")
    {
        request.path_params["event"] = eventName;
        request.body = R"({"values": [1, 2, 3]})";
        postHandler.processEvent(request, response);
        REQUIRE(response.status == httplib::StatusCode::BadRequest_400);
    }

    SECTION("Empty values in payload")
    {
        request.path_params["event"] = eventName;
        request.body = R"({"date": 1, "values": []})";
        postHandler.processEvent(request, response);
        REQUIRE(response.status == httplib::StatusCode::OK_200);
    }

    SECTION("Invalid date in payload")
    {
        request.path_params["event"] = eventName;
        request.body = R"({"date": "invalid_date", "values": [1, 2, 3]})";
        postHandler.processEvent(request, response);
        REQUIRE(response.status == httplib::StatusCode::BadRequest_400);
    }

    SECTION("Invalid values in payload")
    {
        request.path_params["event"] = eventName;
        request.body = R"({"date": 1, "values": "invalid_values"})";
        postHandler.processEvent(request, response);
        REQUIRE(response.status == httplib::StatusCode::BadRequest_400);
    }

    SECTION("Invalid event name")
    {
        request.path_params["event"] = "";
        request.body = createPayload(1, {1, 2, 3});
        postHandler.processEvent(request, response);
        REQUIRE(response.status == httplib::StatusCode::BadRequest_400);
    }

    SECTION("Invalid event name with special characters")
    {
        request.path_params["event"] = "Test@Event";
        request.body = createPayload(1, {1, 2, 3});
        postHandler.processEvent(request, response);
        REQUIRE(response.status == httplib::StatusCode::BadRequest_400);
    }

    SECTION("Invalid event name with spaces")
    {
        request.path_params["event"] = "Test Event";
        request.body = createPayload(1, {1, 2, 3});
        postHandler.processEvent(request, response);
        REQUIRE(response.status == httplib::StatusCode::BadRequest_400);
    }
}
