#include "ScriptRunner.h"

#include "Display.h"
#include "EntrySensor.h"

#include <chrono>
#include <sstream>
#include <thread>

using namespace std;

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
        bool valid = !tooManyArguments;
        if (command == "ENTRY" || command == "EXIT") {
            valid = valid && EntrySensor{}.detectVehicle(argument).has_value();
        } else if (command == "WAIT" || command == "AUTO") {
            valid = valid && parseNonNegativeInt(argument, number);
        } else if (command == "STATUS" || command == "QUIT") {
            valid = valid && argument.empty();
        } else {
            valid = false;
        }

        if (!valid) {
            errors << "Script line " << lineNumber << ": invalid command: " << line << '\n';
            return false;
        }

        if (command == "ENTRY") {
            controller.handleVehicleEntry(argument);
        } else if (command == "EXIT") {
            controller.handleVehicleExit(argument);
        } else if (command == "STATUS") {
            Display::showStatus(controller);
        } else if (command == "WAIT") {
            this_thread::sleep_for(chrono::milliseconds(number));
        } else if (command == "AUTO") {
            sensor.run(controller, number, intervalMs);
        } else { // QUIT
            return true;
        }
    }

    if (input.bad()) {
        errors << "Script read error\n";
        return false;
    }
    return true;
}
