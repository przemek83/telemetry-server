#include <httplib.h>
#include "nlohmann/json_fwd.hpp"

class Telemetry;

class PostHandler
{
public:
    explicit PostHandler(Telemetry& telemetry);

    void processEvent(const httplib::Request& req, httplib::Response& res);

private:
    static std::tuple<bool, int, std::vector<int>> parsePayload(
        const std::string& payload, httplib::Response& res);

    static std::pair<bool, std::vector<int>> parseValues(
        const nlohmann::json& parsedData, httplib::Response& res);

    static std::pair<bool, int> parseDate(const nlohmann::json& parsedData,
                                          httplib::Response& res);

    static bool isValidEventName(const std::string& event);

    static void raiseError(httplib::Response& res,
                           const std::string& errorMessage);

    Telemetry& telemetry_;
};
