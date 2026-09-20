#include "ParkingController.h"
#include "Logger.h"
#include "Config.h"
#include <thread>

ParkingController::ParkingController(int totalSlots): entryBarrier("Entry Barrier"), exitBarrier("Exit Barrier") {
    slots.reserve(totalSlots);
    for (int i = 1; i <= totalSlots; ++i) {
        slots.push_back(ParkingSlot(i));
    }
}

int ParkingController::findFreeSlotIndex() const{
    for (int i = 0; i < slots.size(); ++i) {
        if (slots[i].isFree()) {
            return i;
        }
    }
    return -1;
}

int ParkingController::findSlotIndexByVehicle(const std::string& vehicleId) const {
    std::map<std::string, int>::const_iterator it = vehicleToSlot.find(vehicleId);
    if (it == vehicleToSlot.end()) {
        return -1;
    }
    return it->second - 1;
}

void ParkingController::validateInvariants() const {
    int occupied = getOccupiedCount();
    int total = getTotalSlots();
    int available = getAvailableCount();
    if (occupied > total || available < 0) {
        Logger::getInstance().logError(
            "INVARIANT VIOLATION: occupied=" + std::to_string(occupied) +
            " total=" + std::to_string(total) + " available=" + std::to_string(available));
    }
}

void ParkingController::admitFromQueueIfPossible() {
    if (waitingQueue.empty()) {
        return;
    }
    if (findFreeSlotIndex() == -1) {
        return;
    }
    std::string nextVehicle = waitingQueue.front();
    waitingQueue.pop();
    Logger::getInstance().logInfo("Co cho trong, moi xe dang cho: " + nextVehicle);
    handleVehicleEntry(nextVehicle);
}

EntryResult ParkingController::handleVehicleEntry(const std::string& rawVehicleId) {
    std::optional<std::string> vehicleIdOpt = entrySensor.detectVehicle(rawVehicleId);
    if (!vehicleIdOpt.has_value()) {
        Logger::getInstance().logError("Entry sensor: input khong hop le: '" + rawVehicleId + "'");
        return EntryResult::REJECTED_INVALID_INPUT;
    }
    const std::string vehicleId = *vehicleIdOpt;

    if (vehicleToSlot.find(vehicleId) != vehicleToSlot.end()) {
        Logger::getInstance().logError("Vehicle " + vehicleId + " da co trong bai (tranh occupy trung).");
        return EntryResult::REJECTED_DUPLICATE_VEHICLE;
    }

    int freeIndex = findFreeSlotIndex();
    if (freeIndex == -1) {
        entryBarrier.close();
        Logger::getInstance().logInfo("PARKING FULL - tu choi xe " + vehicleId);
        waitingQueue.push(vehicleId); 
        return EntryResult::REJECTED_FULL;
    }

    entryBarrier.open();
    std::this_thread::sleep_for(std::chrono::milliseconds(Config::VEHICLE_PASSAGE_MS));
    slots[freeIndex].occupy(vehicleId);
    vehicleToSlot[vehicleId] = slots[freeIndex].getId();

    Logger::getInstance().logEntry(vehicleId, slots[freeIndex].getId());
    entryBarrier.close();
    validateInvariants();
    return EntryResult::SUCCESS;
}

int ParkingController::getTotalSlots() const {
    return slots.size();
}

int ParkingController::getAvailableCount() const {
    return getTotalSlots() - getOccupiedCount();
}

int ParkingController::getOccupiedCount() const {
    int count = 0;
    for (const ParkingSlot& slot : slots) {
        if (!slot.isFree()) {
            ++count;
        }
    }
    return count;
}

const std::vector<ParkingSlot>& ParkingController::getSlots() const {
    return slots;
}

const Barrier& ParkingController::getEntryBarrier() const {
    return entryBarrier;
}

const Barrier& ParkingController::getExitBarrier() const {
    return exitBarrier;
}

int ParkingController::getWaitingQueueSize() const {
    return waitingQueue.size();
}


