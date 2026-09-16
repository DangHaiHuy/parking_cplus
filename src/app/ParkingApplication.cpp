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

namespace {

int readMenuChoice() {
    std::string line;
    if (!std::getline(std::cin, line)) {
        return 0;
    }

    int choice = -1;
    return parseNonNegativeInt(line, choice) ? choice : -1;
}

std::string readVehicleId(const std::string &prompt) {
    std::cout << prompt;
    std::string vehicleId;
    std::getline(std::cin, vehicleId);
    return vehicleId;
}

void printEntryResult(EntryResult result, const std::string &vehicleId) {
    switch (result) {
    case EntryResult::SUCCESS:
        std::cout << "-> Xe " << vehicleId << " da vao bai thanh cong.\n";
        break;
    case EntryResult::REJECTED_FULL:
        std::cout << "-> PARKING FULL. Xe " << vehicleId << " duoc xep vao hang cho.\n";
        break;
    case EntryResult::REJECTED_INVALID_INPUT:
        std::cout << "-> Input khong hop le, tu choi xu ly.\n";
        break;
    case EntryResult::REJECTED_DUPLICATE_VEHICLE:
        std::cout << "-> Vehicle ID nay da co trong bai.\n";
        break;
    }
}

void printExitResult(ExitResult result, const std::string &vehicleId) {
    switch (result) {
    case ExitResult::SUCCESS:
        std::cout << "-> Xe " << vehicleId << " da roi bai thanh cong.\n";
        break;
    case ExitResult::REJECTED_INVALID_INPUT:
        std::cout << "-> Input khong hop le, tu choi xu ly.\n";
        break;
    case ExitResult::REJECTED_VEHICLE_NOT_FOUND:
        std::cout << "-> Khong tim thay xe " << vehicleId
                  << " trong bai (co the bai dang trong).\n";
        break;
    }
}

} // namespace

int ParkingApplication::run(int argc, char *argv[]) {
    int autoCount = -1;
    int intervalMs = Config::AUTO_INTERVAL_MS;
    std::string scriptPath;

    for (int i = 1; i < argc; ++i) {
        const std::string option = argv[i];
        if (option == "--help") {
            std::cout << "Usage: smart_parking [--auto N | --script FILE] [--interval MS]\n"
                      << "N: number of sensor events; MS: delay between auto events (default 500).\n"
                      << "Script: ENTRY id, EXIT id, STATUS, WAIT ms, AUTO n, QUIT.\n";
            return 0;
        }

        if ((option != "--auto" && option != "--script" && option != "--interval") ||
            i + 1 == argc) {
            std::cerr << "Invalid/missing option: " << option << ". Use --help.\n";
            return 1;
        }

        const std::string value = argv[++i];
        if (option == "--script") {
            if (!scriptPath.empty() || value.empty()) {
                std::cerr << "Invalid/duplicate --script\n";
                return 1;
            }
            scriptPath = value;
            continue;
        }

        int parsed = 0;
        if (!parseNonNegativeInt(value, parsed) ||
            (option == "--auto" && autoCount >= 0)) {
            std::cerr << "Invalid value for " << option << ": " << value << '\n';
            return 1;
        }
        if (option == "--auto") {
            autoCount = parsed;
        } else {
            intervalMs = parsed;
        }
    }

    if (autoCount >= 0 && !scriptPath.empty()) {
        std::cerr << "Use either --auto or --script.\n";
        return 1;
    }

    std::ifstream script;
    if (!scriptPath.empty()) {
        script.open(scriptPath);
        if (!script) {
            std::cerr << "Cannot open script: " << scriptPath << '\n';
            return 1;
        }
    }

    ParkingController controller(Config::DEFAULT_TOTAL_SLOTS);
    Logger::getInstance().logInfo("He thong Smart Parking khoi dong voi " +
                                  std::to_string(Config::DEFAULT_TOTAL_SLOTS) + " slot.");

    if (autoCount >= 0 || !scriptPath.empty()) {
        AutoSensor sensor;
        const bool success = autoCount >= 0
                                 ? (sensor.run(controller, autoCount, intervalMs), true)
                                 : runScript(script, controller, sensor, intervalMs, std::cerr);
        Display::showStatus(controller);
        Logger::getInstance().logInfo("He thong Smart Parking dung.");
        return success ? 0 : 1;
    }

    bool running = true;
    while (running) {
        Display::showMenu();
        switch (readMenuChoice()) {
        case 1: {
            const std::string vehicleId = readVehicleId("Nhap Vehicle ID (vd: 51A-12345): ");
            printEntryResult(controller.handleVehicleEntry(vehicleId), vehicleId);
            break;
        }
        case 2: {
            const std::string vehicleId = readVehicleId("Nhap Vehicle ID can ra: ");
            printExitResult(controller.handleVehicleExit(vehicleId), vehicleId);
            break;
        }
        case 3:
            Display::showStatus(controller);
            break;
        case 0:
            running = false;
            std::cout << "Tam biet!\n";
            break;
        default:
            std::cout << "-> Lua chon khong hop le, vui long chon lai.\n";
            break;
        }
    }

    Logger::getInstance().logInfo("He thong Smart Parking dung.");
    return 0;
}
