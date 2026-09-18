#include "TimeUtils.h"
#include "Config.h"
#include <iomanip>
#include <sstream>

std::string TimeUtils::formatTimestamp(time_t timestamp) {
    if (timestamp == 0) return "N/A";
    
    time_t adjusted = timestamp + Config::UTC_OFFSET_MINUTES * 60;
    std::tm* timeinfo = std::gmtime(&adjusted);
    if (!timeinfo) return "Error";
    
    std::ostringstream oss;
    oss << std::put_time(timeinfo, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}
