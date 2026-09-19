#include "TimeUtils.h"
#include "Config.h"
#include <ctime>

using namespace std;

string TimeUtils::formatTimestamp(time_t timestamp) {
    if (timestamp == 0) return "N/A";
    
    time_t adjusted = timestamp + Config::UTC_OFFSET_MINUTES * 60;
    tm* timeinfo = gmtime(&adjusted);
    if (!timeinfo) return "Error";
    
    char buffer[20]; // "YYYY-MM-DD HH:MM:SS"
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
    return string(buffer);
}
