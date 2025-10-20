#include <httplib.h>

#include "EventHandler.h"
#include "GetHandler.h"
#include "PostHandler.h"
#include "SimpleLogger.h"
#include "Telemetry.h"

namespace
{
using httplib::Request;
using httplib::Response;

std::function<void(const Request& req, Response& res)> wrapHandler(
    EventHandler& handler)
{
    return [&handler](const Request& req, Response& res)
    { handler.processEvent(req, res); };
}
}  // namespace

int main()
{
    std::string address("0.0.0.0");
    const int port{8080};

    SimpleLogger logger;
    logger.info("Starting server on " + address + ":" + std::to_string(port));

    httplib::Server server;
    Telemetry telemetry;

    GetHandler getHandler(telemetry, logger);
    server.Get("/paths/:event/meanLength", wrapHandler(getHandler));

    PostHandler postHandler(telemetry, logger);
    server.Post("/paths/:event", wrapHandler(postHandler));

    if (!server.listen(address, port))
    {
        logger.error("Failed to start server on " + address + ":" +
                     std::to_string(port));
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
