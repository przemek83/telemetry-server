#include <functional>
#include <map>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

class Telemetry
{
public:
    // Telemetry() = default;
    // ~Telemetry() = default;

    // Telemetry(const Telemetry&) = delete;
    // Telemetry(Telemetry&&) = delete;
    // Telemetry& operator=(const Telemetry&) = delete;
    // Telemetry& operator=(Telemetry&&) = delete;

    void addEntry(const std::string& name, int date,
                  const std::vector<int>& values);

    int computeMean(const std::string& name, int fromDate = DATE_NOT_SET,
                    int toDate = DATE_NOT_SET);

    static const int DATE_NOT_SET{-1};

private:
    static int compute(const std::map<int, std::vector<int>>& entries,
                       const std::function<bool(int)>& shouldIncludeDate);

    std::mutex mutex_;

    std::unordered_map<std::string, std::map<int, std::vector<int>>> entries_;
};
