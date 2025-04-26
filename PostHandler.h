#include <httplib.h>

class Telemetry;

class PostHandler
{
public:
    explicit PostHandler(Telemetry& telemetry);

    void processEvent(const httplib::Request& req, httplib::Response& res);

private:
    Telemetry& telemetry_;
};
