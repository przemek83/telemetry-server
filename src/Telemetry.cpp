#include "Telemetry.h"

#include <functional>
#include <iostream>
#include <mutex>

void Telemetry::addEntry(const std::string& name, int date,
                         const std::vector<int>& values)
{
    std::lock_guard lock(mutex_);
    std::map<int, std::vector<int>>& category{entries_[name]};
    std::vector<int>& entries{category[date]};
    entries.insert(entries.end(), values.begin(), values.end());
}

int Telemetry::computeMean(const std::string& name, int fromDate, int toDate)
{
    const bool fromIsSet{fromDate != DATE_NOT_SET};
    const bool toIsSet{toDate != DATE_NOT_SET};
    std::function<bool(int)> dateFilter{[]([[maybe_unused]] int date)
                                        { return true; }};

    if (fromIsSet && !toIsSet)
        dateFilter = [from = fromDate](int date) { return date >= from; };

    if (!fromIsSet && toIsSet)
        dateFilter = [to = toDate](int date) { return date <= to; };

    if (fromIsSet && toIsSet)
        dateFilter = [from = fromDate, to = toDate](int date)
        { return date >= from && date <= to; };

    std::lock_guard lock(mutex_);
    const std::map<int, std::vector<int>>& category{entries_[name]};
    return compute(category, dateFilter);
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
            // std::cout << "Adding value: " << value << " for date: " << date
            //           << std::endl;
            sum += value;
            ++count;
        }
    }

    std::cout << "Sum: " << sum << " Count: " << count << std::endl;

    return count == 0 ? 0 : sum / count;
}