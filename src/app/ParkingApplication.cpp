#include "ParkingApplication.h"

#include "AutoSensor.h"
#include "Config.h"
#include "Display.h"
#include "Logger.h"
#include "ParkingController.h"
#include "ScriptRunner.h"

#include <fstream>
#include <iostream>
#include <string>

using namespace std;

namespace {

void showHelp() {
    cout << "Usage: smart_parking [--auto N | --script FILE] [--interval MS]\n";
    cout << "--auto N      Tao N su kien xe tu dong.\n";
    cout << "--script FILE Doc lenh tu file script.\n";
    cout << "--interval MS Thoi gian giua cac xe tu dong.\n";
}

int readMenuChoice() {
    string text;
    int choice = -1;

    if (!getline(cin, text)) {
        return 0;
    }
    if (!parseNonNegativeInt(text, choice)) {
        return -1;
    }
    return choice;
}

string readVehicleId(const string &message) {
    string vehicleId;
    cout << message;
    getline(cin, vehicleId);
    return vehicleId;
}

void showEntryResult(EntryResult result, const string &vehicleId) {
    if (result == EntryResult::SUCCESS) {
        cout << "-> Xe " << vehicleId << " da vao bai thanh cong.\n";
    } else if (result == EntryResult::REJECTED_FULL) {
        cout << "-> PARKING FULL. Xe " << vehicleId << " duoc xep vao hang cho.\n";
    } else if (result == EntryResult::REJECTED_INVALID_INPUT) {
        cout << "-> Input khong hop le, tu choi xu ly.\n";
    } else {
        cout << "-> Vehicle ID nay da co trong bai.\n";
    }
}

void showExitResult(ExitResult result, const string &vehicleId) {
    if (result == ExitResult::SUCCESS) {
        cout << "-> Xe " << vehicleId << " da roi bai thanh cong.\n";
    } else if (result == ExitResult::REJECTED_INVALID_INPUT) {
        cout << "-> Input khong hop le, tu choi xu ly.\n";
    } else {
        cout << "-> Khong tim thay xe " << vehicleId << " trong bai.\n";
    }
}

void runMenu(ParkingController &controller) {
    bool running = true;

    while (running) {
        Display::showMenu();
        int choice = readMenuChoice();

        if (choice == 1) {
            string vehicleId = readVehicleId("Nhap Vehicle ID (vd: 51A-12345): ");
            EntryResult result = controller.handleVehicleEntry(vehicleId);
            showEntryResult(result, vehicleId);
        } else if (choice == 2) {
            string vehicleId = readVehicleId("Nhap Vehicle ID can ra: ");
            ExitResult result = controller.handleVehicleExit(vehicleId);
            showExitResult(result, vehicleId);
        } else if (choice == 3) {
            Display::showStatus(controller);
        } else if (choice == 0) {
            running = false;
            cout << "Tam biet!\n";
        } else {
            cout << "-> Lua chon khong hop le, vui long chon lai.\n";
        }
    }
}

} // namespace

int ParkingApplication::run(int argc, char *argv[]) {
    int autoCount = -1;
    int intervalMs = Config::AUTO_INTERVAL_MS;
    string scriptPath;

    for (int i = 1; i < argc; ++i) {
        string option = argv[i];

        if (option == "--help") {
            showHelp();
            return 0;
        }

        if (i + 1 == argc) {
            cerr << "Thieu gia tri cho " << option << '\n';
            return 1;
        }

        string value = argv[++i];

        if (option == "--auto") {
            if (autoCount >= 0 || !parseNonNegativeInt(value, autoCount)) {
                cerr << "Gia tri --auto khong hop le.\n";
                return 1;
            }
        } else if (option == "--script") {
            if (!scriptPath.empty() || value.empty()) {
                cerr << "Duong dan script khong hop le.\n";
                return 1;
            }
            scriptPath = value;
        } else if (option == "--interval") {
            if (!parseNonNegativeInt(value, intervalMs)) {
                cerr << "Gia tri --interval khong hop le.\n";
                return 1;
            }
        } else {
            cerr << "Lua chon khong hop le: " << option << '\n';
            return 1;
        }
    }

    if (autoCount >= 0 && !scriptPath.empty()) {
        cerr << "Chi dung --auto hoac --script.\n";
        return 1;
    }

    ifstream script;
    if (!scriptPath.empty()) {
        script.open(scriptPath);
        if (!script.is_open()) {
            cerr << "Khong mo duoc script: " << scriptPath << '\n';
            return 1;
        }
    }

    ParkingController controller(Config::DEFAULT_TOTAL_SLOTS);
    Logger::getInstance().logInfo("He thong Smart Parking khoi dong voi " +
                                  to_string(Config::DEFAULT_TOTAL_SLOTS) + " slot.");

    if (autoCount >= 0) {
        AutoSensor sensor;
        sensor.run(controller, autoCount, intervalMs);
        Display::showStatus(controller);
    } else if (!scriptPath.empty()) {
        AutoSensor sensor;
        bool success = runScript(script, controller, sensor, intervalMs, cerr);
        Display::showStatus(controller);
        Logger::getInstance().logInfo("He thong Smart Parking dung.");

        if (success) {
            return 0;
        }
        return 1;
    } else {
        runMenu(controller);
    }

    Logger::getInstance().logInfo("He thong Smart Parking dung.");
    return 0;
}
