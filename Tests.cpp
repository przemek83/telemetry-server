#include "Tests.h"

#include <cassert>

#include "Telemetry.h"

namespace tests
{
void runTelemetryTests()
{
    Telemetry telemetry;

    const std::string eventName{"TestEvent"};
    const std::string otherEventName{"OtherTestEvent"};

    assert(telemetry.computeMean(eventName) == 0);

    telemetry.addEntry(eventName, 1, {3, 45, 78, 3});
    assert(telemetry.computeMean(eventName) == 32);
    assert(telemetry.computeMean(eventName, 2, 3) == 0);
    assert(telemetry.computeMean(eventName, Telemetry::DATE_NOT_SET, 3) == 32);
    assert(telemetry.computeMean(eventName, 1) == 32);

    telemetry.addEntry(eventName, 2, {42, 34, 12});
    assert(telemetry.computeMean(eventName) == 31);
    assert(telemetry.computeMean(eventName, 2, 3) == 29);
    assert(telemetry.computeMean(eventName, Telemetry::DATE_NOT_SET, 3) == 31);
    assert(telemetry.computeMean(eventName, 2) == 29);

    telemetry.addEntry(eventName, 3, {13, 123, 54, 31, 9});
    assert(telemetry.computeMean(eventName) == 37);
    assert(telemetry.computeMean(eventName, 1, 2) == 31);
    assert(telemetry.computeMean(eventName, 1, 3) == 37);
    assert(telemetry.computeMean(eventName, 4, 10) == 0);
    assert(telemetry.computeMean(eventName, Telemetry::DATE_NOT_SET, 2) == 31);
    assert(telemetry.computeMean(eventName, 2) == 39);

    telemetry.addEntry(otherEventName, 1, {10, 20, 10, 20});
    assert(telemetry.computeMean(eventName) == 37);
    assert(telemetry.computeMean(otherEventName) == 15);
}

void runServerTests() {}
}  // namespace tests