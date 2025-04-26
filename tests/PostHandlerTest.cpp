#include <httplib.h>
#include <catch2/catch_test_macros.hpp>
#include <nlohmann/json.hpp>

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
    const std::string eventName{"TestEvent"};
    PostHandler postHandler(telemetry);

    SECTION("Posting data and verifying mean")
    {
        request.path_params["event"] = eventName;
        request.body = createPayload(1, {3, 45, 78, 3});
        postHandler.processEvent(request, response);
        REQUIRE(response.status == httplib::StatusCode::OK_200);
        REQUIRE(telemetry.computeMean(eventName) == 32);

        request.path_params["event"] = eventName;
        request.body = createPayload(2, {23, 43, 123, 13});
        postHandler.processEvent(request, response);
        REQUIRE(response.status == httplib::StatusCode::OK_200);
        REQUIRE(telemetry.computeMean(eventName) == 41);
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
        request.body = R"({"date": 1})";  // Missing "values"
        postHandler.processEvent(request, response);
        REQUIRE(response.status == httplib::StatusCode::BadRequest_400);
    }

    SECTION("Missing date in payload")
    {
        request.path_params["event"] = eventName;
        request.body = R"({"values": [1, 2, 3]})";  // Missing "date"
        postHandler.processEvent(request, response);
        REQUIRE(response.status == httplib::StatusCode::BadRequest_400);
    }

    SECTION("Empty values in payload")
    {
        request.path_params["event"] = eventName;
        request.body = R"({"date": 1, "values": []})";  // Empty "values"
        postHandler.processEvent(request, response);
        REQUIRE(response.status == httplib::StatusCode::BadRequest_400);
    }

    SECTION("Invalid date in payload")
    {
        request.path_params["event"] = eventName;
        request.body =
            R"({"date": "invalid_date", "values": [1, 2, 3]})";  // Invalid date
        postHandler.processEvent(request, response);
        REQUIRE(response.status == httplib::StatusCode::BadRequest_400);
    }

    SECTION("Invalid values in payload")
    {
        request.path_params["event"] = eventName;
        request.body =
            R"({"date": 1, "values": "invalid_values"})";  // Invalid values
        postHandler.processEvent(request, response);
        REQUIRE(response.status == httplib::StatusCode::BadRequest_400);
    }

    SECTION("Invalid event name")
    {
        request.path_params["event"] = "";  // Invalid event name
        request.body = createPayload(1, {1, 2, 3});
        postHandler.processEvent(request, response);
        REQUIRE(response.status == httplib::StatusCode::BadRequest_400);
    }

    SECTION("Invalid event name with special characters")
    {
        request.path_params["event"] = "Test@Event";  // Invalid event name
        request.body = createPayload(1, {1, 2, 3});
        postHandler.processEvent(request, response);
        REQUIRE(response.status == httplib::StatusCode::BadRequest_400);
    }

    SECTION("Invalid event name with spaces")
    {
        request.path_params["event"] = "Test Event";  // Invalid event name
        request.body = createPayload(1, {1, 2, 3});
        postHandler.processEvent(request, response);
        REQUIRE(response.status == httplib::StatusCode::BadRequest_400);
    }
}