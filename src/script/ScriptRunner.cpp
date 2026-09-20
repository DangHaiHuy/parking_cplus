#include "ScriptRunner.h"
#include "Display.h"
#include "EntrySensor.h"

#include <chrono>
#include <iostream>
#include <sstream>
#include <thread>

using namespace std;


// Hien thi loi khi script sai
bool showScriptError(
    ostream& errors,
    int lineNumber,
    const string& line
) {
    errors << "Script line "
           << lineNumber
           << ": invalid command: "
           << line
           << '\n';

    return false;
}


// Chuyen chuoi thanh so nguyen khong am
bool parseNonNegativeInt(
    const string& value,
    int& result
) {
    if (value.empty()) {
        return false;
    }

    stringstream input(value);

    if (!(input >> result)) {
        return false;
    }

    if (result < 0) {
        return false;
    }

    // Kiem tra xem sau so con ky tu thua hay khong
    char extra;

    if (input >> extra) {
        return false;
    }

    return true;
}


// Chay cac lenh trong file script
bool runScript(
    istream& input,
    ParkingController& controller,
    AutoSensor& sensor,
    int intervalMs,
    ostream& errors
) {
    string line;
    int lineNumber = 0;

    while (getline(input, line)) {

        lineNumber++;

        // Bo BOM o dau file neu co
        if (lineNumber == 1 &&
            line.compare(0, 3, "\xEF\xBB\xBF") == 0) {

            line.erase(0, 3);
        }

        // Bo phan comment sau dau #
        size_t comment = line.find('#');

        if (comment != string::npos) {
            line.erase(comment);
        }

        // Tach lenh va tham so
        stringstream words(line);

        string command;
        string argument;
        string extra;

        words >> command;

        // Dong trong
        if (command.empty()) {
            continue;
        }

        words >> argument;

        // Kiem tra co tham so thu 2 hay khong
        if (words >> extra) {
            return showScriptError(
                errors,
                lineNumber,
                line
            );
        }

        // LENH ENTRY
        if (command == "ENTRY") {

            if (!EntrySensor{}.detectVehicle(argument).has_value()) {
                return showScriptError(
                    errors,
                    lineNumber,
                    line
                );
            }

            controller.handleVehicleEntry(argument);

            continue;
        }
        // LENH EXIT
        if (command == "EXIT") {

            if (!EntrySensor{}.detectVehicle(argument).has_value()) {
                return showScriptError(
                    errors,
                    lineNumber,
                    line
                );
            }

            controller.handleVehicleExit(argument);

            continue;
        }

        // LENH STATUS
        if (command == "STATUS") {

            if (!argument.empty()) {
                return showScriptError(
                    errors,
                    lineNumber,
                    line
                );
            }

            Display::showStatus(controller);

            continue;
        }
        // LENH WAIT
        if (command == "WAIT") {

            int milliseconds;

            if (!parseNonNegativeInt(argument, milliseconds)) {
                return showScriptError(
                    errors,
                    lineNumber,
                    line
                );
            }

            this_thread::sleep_for(
                chrono::milliseconds(milliseconds)
            );

            continue;
        }
        // LENH AUTO
        if (command == "AUTO") {

            int numberOfVehicles;

            if (!parseNonNegativeInt(argument, numberOfVehicles)) {
                return showScriptError(
                    errors,
                    lineNumber,
                    line
                );
            }

            sensor.run(
                controller,
                numberOfVehicles,
                intervalMs
            );

            continue;
        }

        // LENH QUIT

        if (command == "QUIT") {

            if (!argument.empty()) {
                return showScriptError(
                    errors,
                    lineNumber,
                    line
                );
            }

            return true;
        }

        // Lenh khong ton tai
        return showScriptError(
            errors,
            lineNumber,
            line
        );
    }

    // Kiem tra loi doc file
    if (input.bad()) {
        errors << "Script read error\n";
        return false;
    }

    return true;
}