#include "Logger.h"

#include "TimeUtils.h"

#include <ctime>
#include <filesystem>
#include <iostream>

Logger &Logger::getInstance() {
    static Logger instance;
    return instance;
}

Logger::Logger() {
    std::error_code error;
    std::filesystem::create_directories("logs", error);
    logFile.open("logs/system.log", std::ios::app);
    if (!logFile.is_open()) {
        std::cerr << "[Logger] Khong the mo file logs/system.log de ghi.\n";
    }
}

Logger::~Logger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

std::string Logger::currentTimestamp() {
    return TimeUtils::formatTimestamp(std::time(nullptr));
}

void Logger::writeLine(const std::string &level, const std::string &message) {
    std::lock_guard<std::mutex> lock(logMutex);
    const std::string line = "[" + currentTimestamp() + "] [" + level + "] " + message;
    if (logFile.is_open()) {
        logFile << line << std::endl;
    }
    std::cout << line << std::endl;
}

void Logger::logEntry(const std::string &vehicleId, int slotId) {
    writeLine("ENTRY", "Vehicle " + vehicleId + " occupied slot P" + std::to_string(slotId));
}

void Logger::logExit(const std::string &vehicleId, int slotId, long parkedSeconds) {
    writeLine("EXIT", "Vehicle " + vehicleId + " left slot P" + std::to_string(slotId) +
                          " after " + std::to_string(parkedSeconds) + "s");
}

void Logger::logBarrier(const std::string &barrierName, const std::string &state) {
    writeLine("BARRIER", barrierName + " -> " + state);
}

void Logger::logError(const std::string &message) {
    writeLine("ERROR", message);
}

void Logger::logInfo(const std::string &message) {
    writeLine("INFO", message);
}
