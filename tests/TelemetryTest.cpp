#include <catch2/catch_test_macros.hpp>

#include <src/Telemetry.h>

TEST_CASE("Telemetry", "[rest-server]")
{
    Telemetry telemetry;
    const std::string eventName{"TestEvent"};
    const std::string otherEventName{"OtherTestEvent"};
    const std::vector<int> firstSet{0, 10};
    const int DATE_NOT_SET{Telemetry::DATE_NOT_SET};

    SECTION("Mean on empty")
    {
        REQUIRE(telemetry.computeMean(eventName, Telemetry::DATE_NOT_SET,
                                      Telemetry::DATE_NOT_SET) == 0);
    }

    SECTION("Meam after adding first set")
    {
        telemetry.addEntry(eventName, 1, firstSet);
        REQUIRE(telemetry.computeMean(eventName, Telemetry::DATE_NOT_SET,
                                      Telemetry::DATE_NOT_SET) == 5);
    }

    SECTION("Meam on empty range after adding first set")
    {
        telemetry.addEntry(eventName, 1, firstSet);
        REQUIRE(telemetry.computeMean(eventName, 2, 3) == 0);
    }

    SECTION("Meam to date after adding first set")
    {
        telemetry.addEntry(eventName, 1, firstSet);
        REQUIRE(telemetry.computeMean(eventName, DATE_NOT_SET, 3) == 5);
    }

    SECTION("Meam from date after adding first set")
    {
        telemetry.addEntry(eventName, 1, firstSet);
        REQUIRE(telemetry.computeMean(eventName, 1, Telemetry::DATE_NOT_SET) ==
                5);
    }

    SECTION("Mean after adding multiple entries for the same date")
    {
        telemetry.addEntry(eventName, 1, {10, 20});
        telemetry.addEntry(eventName, 1, {30, 40});
        REQUIRE(telemetry.computeMean(eventName, Telemetry::DATE_NOT_SET,
                                      Telemetry::DATE_NOT_SET) == 25);
    }

    SECTION("Mean with non-overlapping date ranges")
    {
        telemetry.addEntry(eventName, 1, {10, 20});
        telemetry.addEntry(eventName, 3, {30, 40});
        REQUIRE(telemetry.computeMean(eventName, 1, 2) == 15);
        REQUIRE(telemetry.computeMean(eventName, 3, 4) == 35);
    }

    SECTION("Mean with overlapping date ranges")
    {
        telemetry.addEntry(eventName, 1, {10, 20});
        telemetry.addEntry(eventName, 2, {30, 40});
        telemetry.addEntry(eventName, 3, {50, 60});
        REQUIRE(telemetry.computeMean(eventName, 1, 3) == 35);
        REQUIRE(telemetry.computeMean(eventName, 2, 3) == 45);
    }

    SECTION("Mean from date for multiple sets")
    {
        telemetry.addEntry(eventName, 1, {10, 20});
        telemetry.addEntry(eventName, 2, {30, 40});
        telemetry.addEntry(eventName, 3, {50, 60});
        REQUIRE(telemetry.computeMean(eventName, 1, Telemetry::DATE_NOT_SET) ==
                35);
    }

    SECTION("Mean to date for multiple sets")
    {
        telemetry.addEntry(eventName, 1, {10, 20});
        telemetry.addEntry(eventName, 2, {30, 40});
        telemetry.addEntry(eventName, 3, {50, 60});
        REQUIRE(telemetry.computeMean(eventName, DATE_NOT_SET, 2) == 25);
        REQUIRE(telemetry.computeMean(eventName, DATE_NOT_SET, 3) == 35);
    }

    SECTION("Mean with no entries for the given date range")
    {
        telemetry.addEntry(eventName, 1, {10, 20});
        telemetry.addEntry(eventName, 3, {30, 40});
        REQUIRE(telemetry.computeMean(eventName, 4, 5) == 0);
    }

    SECTION("Mean with entries for multiple events")
    {
        telemetry.addEntry(eventName, 1, {10, 20});
        telemetry.addEntry(otherEventName, 1, {30, 40});
        REQUIRE(telemetry.computeMean(eventName, Telemetry::DATE_NOT_SET,
                                      Telemetry::DATE_NOT_SET) == 15);
        REQUIRE(telemetry.computeMean(otherEventName, Telemetry::DATE_NOT_SET,
                                      Telemetry::DATE_NOT_SET) == 35);
    }

    SECTION("Mean with entries spanning multiple dates")
    {
        telemetry.addEntry(eventName, 1, {10, 20});
        telemetry.addEntry(eventName, 2, {30, 40});
        telemetry.addEntry(eventName, 3, {50, 60});
        REQUIRE(telemetry.computeMean(eventName, Telemetry::DATE_NOT_SET,
                                      Telemetry::DATE_NOT_SET) == 35);
    }
}
