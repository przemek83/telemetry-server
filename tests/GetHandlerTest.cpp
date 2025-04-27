#include <httplib.h>
#include <catch2/catch_test_macros.hpp>
#include <nlohmann/json.hpp>

#include <src/GetHandler.h>
#include <src/Telemetry.h>

namespace
{
httplib::Params createParams(int start, int end,
                             std::string resultUnit = "milliseconds")
{
    httplib::Params params;
    params.emplace("resultUnit", resultUnit);
    if (start != Telemetry::DATE_NOT_SET)
        params.emplace("startTimestamp", std::to_string(start));

    if (end != Telemetry::DATE_NOT_SET)
        params.emplace("endTimestamp", std::to_string(end));

    return params;
}

void validateMean(const httplib::Response& response, int expectedMean)
{
    REQUIRE(response.status == httplib::StatusCode::OK_200);
    REQUIRE(!response.body.empty());
    const auto jsonResponse = nlohmann::json::parse(response.body);
    REQUIRE(jsonResponse["mean"] == expectedMean);
}
}  // namespace

TEST_CASE("GetHandler Tests", "[rest-server]")
{
    httplib::Request request;
    httplib::Response response;

    const std::string eventName{"TestEvent"};

    Telemetry telemetry;
    telemetry.addEntry(eventName, 1, {1, 2, 3});
    telemetry.addEntry(eventName, 2, {4, 5, 6});
    telemetry.addEntry(eventName, 3, {7, 8, 9});
    GetHandler getHandler(telemetry);

    SECTION("Get data and verifying mean")
    {
        request.params =
            createParams(Telemetry::DATE_NOT_SET, Telemetry::DATE_NOT_SET);
        request.path_params["event"] = eventName;
        getHandler.processEvent(request, response);
        validateMean(response, 5);
    }

    SECTION("Get data with start and end date")
    {
        request.params = createParams(1, 3);
        request.path_params["event"] = eventName;
        getHandler.processEvent(request, response);
        validateMean(response, 5);
    }

    SECTION("Get data with start date")
    {
        request.params = createParams(2, Telemetry::DATE_NOT_SET);
        request.path_params["event"] = eventName;
        getHandler.processEvent(request, response);
        validateMean(response, 6);
    }

    SECTION("Get data with end date")
    {
        request.params = createParams(Telemetry::DATE_NOT_SET, 2);
        request.path_params["event"] = eventName;
        getHandler.processEvent(request, response);
        validateMean(response, 3);
    }

    SECTION("Get data with invalid event name")
    {
        request.params = createParams(1, 3);
        request.path_params["event"] = "InvalidEvent";
        getHandler.processEvent(request, response);
        validateMean(response, 0);
    }

    SECTION("Get data with no event name")
    {
        request.params = createParams(1, 3);
        request.path_params["event"] = "";
        getHandler.processEvent(request, response);
        REQUIRE(response.status == httplib::StatusCode::BadRequest_400);
    }

    SECTION("Get data with event name with space")
    {
        request.params = createParams(1, 3);
        request.path_params["event"] = "Test Event";
        getHandler.processEvent(request, response);
        REQUIRE(response.status == httplib::StatusCode::BadRequest_400);
    }

    SECTION("Get data with event name with special characters")
    {
        request.params = createParams(1, 3);
        request.path_params["event"] = "TestEvent@123";
        getHandler.processEvent(request, response);
        REQUIRE(response.status == httplib::StatusCode::BadRequest_400);
    }

    SECTION("Get data with no params")
    {
        request.params = httplib::Params{};
        request.path_params["event"] = eventName;
        getHandler.processEvent(request, response);
        REQUIRE(response.status == httplib::StatusCode::BadRequest_400);
    }

    SECTION("Get data with invalid params")
    {
        request.params = createParams(1, 3);
        request.path_params["event"] = eventName;
        request.path_params["invalid"] = "invalid";
        getHandler.processEvent(request, response);
        validateMean(response, 5);
    }

    SECTION("Get data with invalid result unit")
    {
        request.params = createParams(1, 3, "invalidTimeUnit");
        request.path_params["event"] = eventName;
        getHandler.processEvent(request, response);
        REQUIRE(response.status == httplib::StatusCode::BadRequest_400);
    }

    SECTION("Get data with empty result unit")
    {
        request.params = createParams(1, 3, "");
        request.path_params["event"] = eventName;
        getHandler.processEvent(request, response);
        REQUIRE(response.status == httplib::StatusCode::BadRequest_400);
    }
}