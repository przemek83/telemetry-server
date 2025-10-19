#include "Telemetry.h"

#include <mutex>

void Telemetry::addEntry(const std::string& name, int date,
                         const std::vector<int>& values)
{
    std::unique_lock lock(mutex_);
    std::map<int, std::vector<int>>& category{entries_[name]};
    std::vector<int>& entries{category[date]};
    entries.insert(entries.end(), values.begin(), values.end());
}

int Telemetry::computeMean(const std::string& name, int fromDate, int toDate)
{
    const auto filter{createFilter(fromDate, toDate)};

    std::shared_lock lock(mutex_);
    const std::map<int, std::vector<int>>& category{entries_[name]};
    return compute(category, filter);
}

int Telemetry::compute(const std::map<int, std::vector<int>>& entries,
                       const std::function<bool(int)>& shouldIncludeDate)
{
    int sum{0};
    int count{0};
    for (const auto& [date, values] : entries)
    {
        if (!shouldIncludeDate(date))
            continue;

        for (const auto& value : values)
        {
            sum += value;
            ++count;
        }
    }

    return (count == 0) ? 0 : (sum / count);
}

std::function<bool(int)> Telemetry::createFilter(int fromDate, int toDate)
{
    const bool haveFrom{fromDate != DATE_NOT_SET};
    const bool haveTo{toDate != DATE_NOT_SET};
    std::function<bool(int)> dateFilter{[]([[maybe_unused]] int date)
                                        { return true; }};

    if (haveFrom && !haveTo)
        dateFilter = [from = fromDate](int date) { return date >= from; };

    if (!haveFrom && haveTo)
        dateFilter = [to = toDate](int date) { return date <= to; };

    if (haveFrom && haveTo)
        dateFilter = [from = fromDate, to = toDate](int date)
        { return date >= from && date <= to; };

    return dateFilter;
}
