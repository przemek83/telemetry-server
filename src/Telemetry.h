#pragma once

#include <functional>
#include <map>
#include <shared_mutex>
#include <string>
#include <vector>

class Telemetry
{
public:
    void addEntry(const std::string& name, int date,
                  const std::vector<int>& values);

    int computeMean(const std::string& name, int fromDate, int toDate);

    static const int DATE_NOT_SET{-1};

private:
    static int compute(const std::map<int, std::vector<int>>& entries,
                       const std::function<bool(int)>& shouldIncludeDate);

    static std::function<bool(int)> createFilter(int fromDate, int toDate);

    std::shared_mutex mutex_;

    std::map<std::string, std::map<int, std::vector<int>>, std::less<>>
        entries_;
};
