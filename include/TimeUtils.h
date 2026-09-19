#ifndef TIME_UTILS_H
#define TIME_UTILS_H

#include <ctime>
#include <string>

using namespace std;

class TimeUtils {
public:
    TimeUtils();
    string formatTimestamp(time_t timestamp);
};

#endif // TIME_UTILS_H
