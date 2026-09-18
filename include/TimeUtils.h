#ifndef TIME_UTILS_H
#define TIME_UTILS_H

#include <ctime>
#include <string>

class TimeUtils {
public:
    static std::string formatTimestamp(time_t timestamp);
};

#endif // TIME_UTILS_H
