#ifndef PARKING_CONTROLLER_H
#define PARKING_CONTROLLER_H

#include "ParkingSlot.h"
#include "Barrier.h"
#include "EntrySensor.h"
#include "ExitSensor.h"

#include <map>
#include <string>
#include <queue>
#include <vector>

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
    private:
        std::vector<ParkingSlot> slots;
        Barrier entryBarrier;
        Barrier exitBarrier;
        EntrySensor entrySensor;
        ExitSensor exitSensor;

        std::map<std::string, int> vehicleToSlot; 
        std::queue<std::string> waitingQueue;  
        
        int findFreeSlotIndex() const;
        int findSlotIndexByVehicle(const std::string& vehicleId) const;
        void validateInvariants() const; 
        void admitFromQueueIfPossible();

    public:
        ParkingController(int totalSlots);
        EntryResult handleVehicleEntry(const std::string& rawVehicleId);
        ExitResult handleVehicleExit(const std::string& rawVehicleId);

        int getTotalSlots() const;
        int getOccupiedCount() const;
        int getAvailableCount() const;

        const std::vector<ParkingSlot>& getSlots() const;
        const Barrier& getEntryBarrier() const;
        const Barrier& getExitBarrier() const;

        int getWaitingQueueSize() const;
};

#endif 