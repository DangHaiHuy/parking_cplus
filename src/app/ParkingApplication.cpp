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

// Hien thi huong dan
void showHelp() {
    cout << "Usage: smart_parking [--auto N | --script FILE] [--interval MS]\n";
    cout << "--auto N      Tao N su kien xe tu dong.\n";
    cout << "--script FILE Doc lenh tu file script.\n";
    cout << "--interval MS Thoi gian giua cac xe tu dong.\n";
}

// Doc lua chon tu menu
int readMenuChoice() {
    string text;
    int choice;

    if (!getline(cin, text)) {
        return 0;
    }

    if (!parseNonNegativeInt(text, choice)) {
        return -1;
    }

    return choice;
}

// Doc Vehicle ID
string readVehicleId(const string& message) {
    string vehicleId;

    cout << message;
    getline(cin, vehicleId);

    return vehicleId;
}

// Hien thi ket qua xe vao
void showEntryResult(EntryResult result, const string& vehicleId) {
    if (result == EntryResult::SUCCESS) {
        cout << "-> Xe " << vehicleId << " da vao bai thanh cong.\n";
    }
    else if (result == EntryResult::REJECTED_FULL) {
        cout << "-> PARKING FULL. Xe " << vehicleId
             << " duoc xep vao hang cho.\n";
    }
    else if (result == EntryResult::REJECTED_INVALID_INPUT) {
        cout << "-> Input khong hop le, tu choi xu ly.\n";
    }
    else {
        cout << "-> Vehicle ID nay da co trong bai.\n";
    }
}

// Hien thi ket qua xe ra
void showExitResult(ExitResult result, const string& vehicleId) {
    if (result == ExitResult::SUCCESS) {
        cout << "-> Xe " << vehicleId << " da roi bai thanh cong.\n";
    }
    else if (result == ExitResult::REJECTED_INVALID_INPUT) {
        cout << "-> Input khong hop le, tu choi xu ly.\n";
    }
    else {
        cout << "-> Khong tim thay xe " << vehicleId << " trong bai.\n";
    }
}

// Xu ly menu
void runMenu(ParkingController& controller) {
    bool running = true;

    while (running) {
        Display::showMenu();

        int choice = readMenuChoice();

        switch (choice) {
        case 1: {
            string vehicleId =
                readVehicleId("Nhap Vehicle ID (vd : 30A-123.45): ");

            EntryResult result =
                controller.handleVehicleEntry(vehicleId);

            showEntryResult(result, vehicleId);
            break;
        }

        case 2: {
            string vehicleId =
                readVehicleId("Nhap Vehicle ID can ra: ");

            ExitResult result =
                controller.handleVehicleExit(vehicleId);

            showExitResult(result, vehicleId);
            break;
        }

        case 3:
            Display::showStatus(controller);
            break;

        case 0:
            running = false;
            cout << "Tam biet!\n";
            break;

        default:
            cout << "-> Lua chon khong hop le, vui long chon lai.\n";
            break;
        }
    }
}

} // namespace


int ParkingApplication::run(int argc, char* argv[]) {

    int autoCount = -1;
    int intervalMs = Config::AUTO_INTERVAL_MS;
    string scriptPath;

    // Doc tham so khi chay chuong trinh
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

            if (autoCount >= 0 ||
                !parseNonNegativeInt(value, autoCount)) {

                cerr << "Gia tri --auto khong hop le.\n";
                return 1;
            }

        }
        else if (option == "--script") {

            if (!scriptPath.empty() || value.empty()) {

                cerr << "Duong dan script khong hop le.\n";
                return 1;
            }

            scriptPath = value;
        }
        else if (option == "--interval") {

            if (!parseNonNegativeInt(value, intervalMs)) {

                cerr << "Gia tri --interval khong hop le.\n";
                return 1;
            }

        }
        else {

            cerr << "Lua chon khong hop le: " << option << '\n';
            return 1;
        }
    }

    if (autoCount >= 0 && !scriptPath.empty()) {
        cerr << "Chi dung --auto hoac --script.\n";
        return 1;
    }

    // Mo file script neu co
    ifstream script;

    if (!scriptPath.empty()) {

        script.open(scriptPath);

        if (!script.is_open()) {
            cerr << "Khong mo duoc script: "
                 << scriptPath << '\n';

            return 1;
        }
    }

    // Tao he thong bai do xe
    ParkingController controller(
        Config::DEFAULT_TOTAL_SLOTS
    );

    Logger::getInstance().logInfo(
        "He thong Smart Parking khoi dong voi " +
        to_string(Config::DEFAULT_TOTAL_SLOTS) +
        " slot."
    );
    // CHAY CHE DO AUTO
    if (autoCount >= 0) {

        AutoSensor sensor;

        sensor.run(
            controller,
            autoCount,
            intervalMs
        );

        Display::showStatus(controller);
    }

    // CHAY CHE DO SCRIPT
    else if (!scriptPath.empty()) {

        AutoSensor sensor;

        bool success = runScript(
            script,
            controller,
            sensor,
            intervalMs,
            cerr
        );

        Display::showStatus(controller);

        Logger::getInstance().logInfo(
            "He thong Smart Parking dung."
        );

        if (!success) {
            return 1;
        }
    }
    // CHAY CHE DO MENU
    else {
        runMenu(controller);
    }


    // Ket thuc chuong trinh
    Logger::getInstance().logInfo(
        "He thong Smart Parking dung."
    );

    return 0;
}