#ifndef LOGGER_H
#define LOGGER_H

#include <string>

class Logger {
private:
    Logger();

    void writeLine(const std::string& level, const std::string& message);

public:
    static Logger& getInstance();

    void logEntry(const std::string& vehicleId, int slotId);
    void logExit(const std::string& vehicleId, int slotId, long parkedSeconds);
    void logBarrier(const std::string& barrierName, const std::string& state);
    void logError(const std::string& message);
    void logInfo(const std::string& message);
};

#endif