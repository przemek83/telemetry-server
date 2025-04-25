#include <httplib.h>

class PostHandler
{
public:
    void processEvent(const httplib::Request& req, httplib::Response& res);
};
