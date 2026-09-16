#include "Display.h"

#include "TimeUtils.h"

#include <ctime>
#include <iostream>

void Display::showStatus(const ParkingController &controller) {
    std::cout << "========== SMART PARKING ==========\n";
    std::cout << "Time        : " << TimeUtils::formatTimestamp(std::time(nullptr)) << '\n';
    std::cout << "Total Slots : " << controller.getTotalSlots() << '\n';
    std::cout << "Occupied    : " << controller.getOccupiedCount() << '\n';
    std::cout << "Available   : " << controller.getAvailableCount() << "\n\n";

    const auto &slots = controller.getSlots();
    for (std::size_t i = 0; i < slots.size(); ++i) {
        std::cout << slots[i].toDisplayString();
        if ((i + 1) % 5 == 0 || i + 1 == slots.size()) {
            std::cout << '\n';
        } else {
            std::cout << ' ';
        }
    }

    for (const auto &slot : slots) {
        if (!slot.isFree()) {
            std::cout << "P" << slot.getId() << " | " << slot.getVehicleId()
                      << " | Vao: " << TimeUtils::formatTimestamp(slot.getEntryTime())
                      << " | Da do: " << slot.getParkedSeconds() << "s\n";
        }
    }

    std::cout << "\nEntry Barrier : " << controller.getEntryBarrier().stateToString() << '\n';
    std::cout << "Exit Barrier  : " << controller.getExitBarrier().stateToString() << '\n';
    if (controller.getWaitingQueueSize() > 0) {
        std::cout << "Waiting queue : " << controller.getWaitingQueueSize() << " xe\n";
    }
    std::cout << "===================================\n";
}

void Display::showMenu() {
    std::cout << "1. Simulate car entering\n"
              << "2. Simulate car leaving\n"
              << "3. Show status\n"
              << "0. Exit\n"
              << "Chon: ";
}
