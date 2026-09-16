#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <mutex>
#include <string>

class Logger {
public:
    static Logger &getInstance();

    Logger(const Logger &) = delete;
    Logger &operator=(const Logger &) = delete;

    void logEntry(const std::string &vehicleId, int slotId);
    void logExit(const std::string &vehicleId, int slotId, long parkedSeconds);
    void logBarrier(const std::string &barrierName, const std::string &state);
    void logError(const std::string &message);
    void logInfo(const std::string &message);

private:
    Logger();
    ~Logger();

    void writeLine(const std::string &level, const std::string &message);
    static std::string currentTimestamp();

    std::ofstream logFile;
    std::mutex logMutex;
};

#endif // LOGGER_H
