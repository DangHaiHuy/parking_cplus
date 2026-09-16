#include "Logger.h"

#include "TimeUtils.h"

#include <ctime>
#include <filesystem>
#include <iostream>

using namespace std;

Logger &Logger::getInstance() {
    static Logger instance;
    return instance;
}

Logger::Logger() {
    error_code error;
    filesystem::create_directories("logs", error);
    logFile.open("logs/system.log", ios::app);
    if (!logFile.is_open()) {
        cerr << "[Logger] Khong the mo file logs/system.log de ghi.\n";
    }
}

Logger::~Logger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

string Logger::currentTimestamp() {
    return TimeUtils::formatTimestamp(time(nullptr));
}

void Logger::writeLine(const string &level, const string &message) {
    lock_guard<mutex> lock(logMutex);
    string line = "[" + currentTimestamp() + "] [" + level + "] " + message;
    if (logFile.is_open()) {
        logFile << line << endl;
    }
    cout << line << endl;
}

void Logger::logEntry(const string &vehicleId, int slotId) {
    writeLine("ENTRY", "Vehicle " + vehicleId + " occupied slot P" + to_string(slotId));
}

void Logger::logExit(const string &vehicleId, int slotId, long parkedSeconds) {
    writeLine("EXIT", "Vehicle " + vehicleId + " left slot P" + to_string(slotId) +
                          " after " + to_string(parkedSeconds) + "s");
}

void Logger::logBarrier(const string &barrierName, const string &state) {
    writeLine("BARRIER", barrierName + " -> " + state);
}

void Logger::logError(const string &message) {
    writeLine("ERROR", message);
}

void Logger::logInfo(const string &message) {
    writeLine("INFO", message);
}
