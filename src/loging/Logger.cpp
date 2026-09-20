#include "Logger.h"

#include <fstream>
#include <iostream>

using namespace std;

Logger& Logger::getInstance() {
    static Logger logger;
    return logger;
}

Logger::Logger() {
    // File logs/system.log phai ton tai thu muc logs
}

void Logger::writeLine(const string& level, const string& message) {
    ofstream file("logs/system.log", ios::app);

    if (file.is_open()) {
        file << "[" << level << "] " << message << endl;
    }

    cout << "[" << level << "] " << message << endl;
}

void Logger::logEntry(const string& vehicleId, int slotId) {
    writeLine(
        "ENTRY",
        "Vehicle " + vehicleId +
        " occupied slot P" + to_string(slotId)
    );
}

void Logger::logExit(
    const string& vehicleId,
    int slotId,
    long parkedSeconds
) {
    writeLine(
        "EXIT",
        "Vehicle " + vehicleId +
        " left slot P" + to_string(slotId) +
        " after " + to_string(parkedSeconds) + "s"
    );
}

void Logger::logBarrier(
    const string& barrierName,
    const string& state
) {
    writeLine(
        "BARRIER",
        barrierName + " -> " + state
    );
}

void Logger::logError(const string& message) {
    writeLine("ERROR", message);
}

void Logger::logInfo(const string& message) {
    writeLine("INFO", message);
}