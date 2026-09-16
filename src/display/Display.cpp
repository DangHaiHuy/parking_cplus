#include "Display.h"

#include "TimeUtils.h"

#include <ctime>
#include <iostream>

using namespace std;

void Display::showStatus(const ParkingController &controller) {
    cout << "========== SMART PARKING ==========\n";
    cout << "Time        : " << TimeUtils::formatTimestamp(time(nullptr)) << '\n';
    cout << "Total Slots : " << controller.getTotalSlots() << '\n';
    cout << "Occupied    : " << controller.getOccupiedCount() << '\n';
    cout << "Available   : " << controller.getAvailableCount() << "\n\n";

    const auto &slots = controller.getSlots();
    for (size_t i = 0; i < slots.size(); ++i) {
        cout << slots[i].toDisplayString();
        if ((i + 1) % 5 == 0 || i + 1 == slots.size()) {
            cout << '\n';
        } else {
            cout << ' ';
        }
    }

    for (const auto &slot : slots) {
        if (!slot.isFree()) {
            cout << "P" << slot.getId() << " | " << slot.getVehicleId()
                 << " | Vao: " << TimeUtils::formatTimestamp(slot.getEntryTime())
                 << " | Da do: " << slot.getParkedSeconds() << "s\n";
        }
    }

    cout << "\nEntry Barrier : " << controller.getEntryBarrier().stateToString() << '\n';
    cout << "Exit Barrier  : " << controller.getExitBarrier().stateToString() << '\n';
    if (controller.getWaitingQueueSize() > 0) {
        cout << "Waiting queue : " << controller.getWaitingQueueSize() << " xe\n";
    }
    cout << "===================================\n";
}

void Display::showMenu() {
    cout << "1. Simulate car entering\n"
         << "2. Simulate car leaving\n"
         << "3. Show status\n"
         << "0. Exit\n"
         << "Chon: ";
}
