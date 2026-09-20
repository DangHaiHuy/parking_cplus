#include "ScriptRunner.h"

#include "Display.h"
#include "EntrySensor.h"

#include <chrono>
#include <sstream>
#include <thread>

using namespace std;

static bool showScriptError(ostream &errors, int lineNumber, const string &line) {
    errors << "Script line " << lineNumber << ": invalid command: " << line << '\n';
    return false;
}

bool parseNonNegativeInt(const string &value, int &result) {
    if (value.empty()) {
        return false;
    }

    stringstream input(value);
    char extra;
    if (!(input >> result) || result < 0) {
        return false;
    }
    return !(input >> extra);
}

bool runScript(istream &input, ParkingController &controller,
               AutoSensor &sensor, int intervalMs, ostream &errors) {
    string line;
    int lineNumber = 0;
    while (getline(input, line)) {
        ++lineNumber;
        if (lineNumber == 1 && line.compare(0, 3, "\xEF\xBB\xBF") == 0) {
            line.erase(0, 3);
        }

        size_t comment = line.find('#');
        if (comment != string::npos) {
            line.erase(comment);
        }

        istringstream words(line);
        string command;
        string argument;
        string extra;
        if (!(words >> command)) {
            continue;
        }
        words >> argument;
        bool tooManyArguments = false;
        if (words >> extra) {
            tooManyArguments = true;
        }

        int number = 0;
        if (tooManyArguments) {
            return showScriptError(errors, lineNumber, line);
        }
        if (command == "ENTRY") {
            if (!EntrySensor{}.detectVehicle(argument).has_value()) {
                return showScriptError(errors, lineNumber, line);
            }
            controller.handleVehicleEntry(argument);
            continue;
        }
        if (command == "EXIT") {
            if (!EntrySensor{}.detectVehicle(argument).has_value()) {
                return showScriptError(errors, lineNumber, line);
            }
            controller.handleVehicleExit(argument);
            continue;
        }
        if (command == "STATUS") {
            if (!argument.empty()) {
                return showScriptError(errors, lineNumber, line);
            }
            Display::showStatus(controller);
            continue;
        }
        if (command == "WAIT") {
            if (!parseNonNegativeInt(argument, number)) {
                return showScriptError(errors, lineNumber, line);
            }
            this_thread::sleep_for(chrono::milliseconds(number));
            continue;
        }
        if (command == "AUTO") {
            if (!parseNonNegativeInt(argument, number)) {
                return showScriptError(errors, lineNumber, line);
            }
            sensor.run(controller, number, intervalMs);
            continue;
        }
        if (command == "QUIT") {
            if (!argument.empty()) {
                return showScriptError(errors, lineNumber, line);
            }
            return true;
        }
        return showScriptError(errors, lineNumber, line);
    }

    if (input.bad()) {
        errors << "Script read error\n";
        return false;
    }
    return true;
}
