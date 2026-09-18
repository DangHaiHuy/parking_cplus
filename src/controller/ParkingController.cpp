#include "ParkingController.h"
#include "Logger.h"

ParkingController::ParkingController(int totalSlots) 
    : totalSlots(totalSlots), occupiedCount(0), waitingQueueSize(0),
      entryBarrier("Entry Barrier"), exitBarrier("Exit Barrier") 
{
    for (int i = 1; i <= totalSlots; ++i) {
        slots.emplace_back(i);
    }
}

int ParkingController::getTotalSlots() const { return totalSlots; }
int ParkingController::getOccupiedCount() const { return occupiedCount; }
int ParkingController::getAvailableCount() const { return totalSlots - occupiedCount; }

const std::vector<ParkingSlot>& ParkingController::getSlots() const { return slots; }
const Barrier& ParkingController::getEntryBarrier() const { return entryBarrier; }
const Barrier& ParkingController::getExitBarrier() const { return exitBarrier; }

int ParkingController::getWaitingQueueSize() const { return waitingQueueSize; }

EntryResult ParkingController::handleVehicleEntry(const std::string& vehicleId) {
    if (vehicleId.empty()) {
        Logger::getInstance().logError("Invalid vehicle ID on entry.");
        return EntryResult::REJECTED_INVALID_INPUT;
    }
    
    for (const auto& slot : slots) {
        if (!slot.isFree() && slot.getVehicleId() == vehicleId) {
            Logger::getInstance().logError("Vehicle " + vehicleId + " is already in the parking lot.");
            return EntryResult::REJECTED_DUPLICATE_VEHICLE;
        }
    }
    
    if (getAvailableCount() == 0) {
        waitingQueueSize++;
        Logger::getInstance().logInfo("Vehicle " + vehicleId + " rejected (PARKING FULL). Added to waiting queue.");
        return EntryResult::REJECTED_FULL;
    }
    
    entryBarrier.raise();
    
    int assignedSlotId = -1;
    for (auto& slot : slots) {
        if (slot.isFree()) {
            slot.occupy(vehicleId);
            occupiedCount++;
            assignedSlotId = slot.getId();
            Logger::getInstance().logEntry(vehicleId, assignedSlotId);
            break;
        }
    }
    
    entryBarrier.lower();
    return EntryResult::SUCCESS;
}

ExitResult ParkingController::handleVehicleExit(const std::string& vehicleId) {
    if (vehicleId.empty()) {
        Logger::getInstance().logError("Invalid vehicle ID on exit.");
        return ExitResult::REJECTED_INVALID_INPUT;
    }
    
    for (auto& slot : slots) {
        if (!slot.isFree() && slot.getVehicleId() == vehicleId) {
            exitBarrier.raise();
            
            long parkedSeconds = slot.getParkedSeconds();
            int slotId = slot.getId();
            slot.free();
            occupiedCount--;
            
            Logger::getInstance().logExit(vehicleId, slotId, parkedSeconds);
            
            exitBarrier.lower();
            
            if (waitingQueueSize > 0) {
                waitingQueueSize--;
            }
            
            return ExitResult::SUCCESS;
        }
    }
    
    Logger::getInstance().logError("Vehicle " + vehicleId + " not found on exit.");
    return ExitResult::REJECTED_VEHICLE_NOT_FOUND;
}
