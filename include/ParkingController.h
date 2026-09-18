#ifndef PARKING_CONTROLLER_H
#define PARKING_CONTROLLER_H

#include "Barrier.h"
#include "ParkingSlot.h"
#include <vector>
#include <string>

enum class EntryResult {
    SUCCESS,
    REJECTED_FULL,
    REJECTED_INVALID_INPUT,
    REJECTED_DUPLICATE_VEHICLE
};

enum class ExitResult {
    SUCCESS,
    REJECTED_INVALID_INPUT,
    REJECTED_VEHICLE_NOT_FOUND
};

class ParkingController {
public:
    explicit ParkingController(int totalSlots);

    int getTotalSlots() const;
    int getOccupiedCount() const;
    int getAvailableCount() const;
    
    const std::vector<ParkingSlot>& getSlots() const;
    const Barrier& getEntryBarrier() const;
    const Barrier& getExitBarrier() const;
    
    int getWaitingQueueSize() const;

    EntryResult handleVehicleEntry(const std::string& vehicleId);
    ExitResult handleVehicleExit(const std::string& vehicleId);

private:
    int totalSlots;
    int occupiedCount;
    int waitingQueueSize;
    
    std::vector<ParkingSlot> slots;
    Barrier entryBarrier;
    Barrier exitBarrier;
};

#endif // PARKING_CONTROLLER_H
