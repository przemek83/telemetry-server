#include <httplib.h>

class Telemetry;

class GetHandler
{
public:
    explicit GetHandler(Telemetry& telemetry);

    void processEvent(const httplib::Request& req, httplib::Response& res);

private:
    Telemetry& telemetry_;
};