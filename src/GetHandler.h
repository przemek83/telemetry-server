#pragma once

#include "EventHandler.h"

class Telemetry;

class GetHandler : public EventHandler
{
public:
    explicit GetHandler(Telemetry& telemetry);

    void processEvent(const httplib::Request& req,
                      httplib::Response& res) override;
};