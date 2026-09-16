#include "ScriptRunner.h"

#include "Display.h"
#include "EntrySensor.h"

#include <charconv>
#include <chrono>
#include <sstream>
#include <thread>

bool parseNonNegativeInt(const std::string &value, int &result) {
    if (value.empty()) {
        return false;
    }

    const auto parsed = std::from_chars(value.data(), value.data() + value.size(), result);
    return parsed.ec == std::errc{} && parsed.ptr == value.data() + value.size() && result >= 0;
}

bool runScript(std::istream &input, ParkingController &controller,
               AutoSensor &sensor, int intervalMs, std::ostream &errors) {
    std::string line;
    int lineNumber = 0;
    while (std::getline(input, line)) {
        ++lineNumber;
        if (lineNumber == 1 && line.compare(0, 3, "\xEF\xBB\xBF") == 0) {
            line.erase(0, 3);
        }

        const auto comment = line.find('#');
        if (comment != std::string::npos) {
            line.erase(comment);
        }

        std::istringstream words(line);
        std::string command;
        std::string argument;
        std::string extra;
        if (!(words >> command)) {
            continue;
        }
        words >> argument;
        const bool tooManyArguments = static_cast<bool>(words >> extra);

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
            std::this_thread::sleep_for(std::chrono::milliseconds(number));
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
