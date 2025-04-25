#include <httplib.h>

class GetHandler
{
public:
    void processEvent(const httplib::Request& req, httplib::Response& res);
};