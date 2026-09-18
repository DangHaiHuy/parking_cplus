#include "ParkingSlot.h"

#include <iomanip>
#include <sstream>

ParkingSlot::ParkingSlot(int id) : id(id), statusIsFree(true), entryTime(0) {}

int ParkingSlot::getId() const {
    return id;
}

bool ParkingSlot::isFree() const {
    return statusIsFree;
}

const std::string& ParkingSlot::getVehicleId() const {
    return vehicleId;
}

time_t ParkingSlot::getEntryTime() const {
    return entryTime;
}

long ParkingSlot::getParkedSeconds() const {
    if (statusIsFree) return 0;
    return static_cast<long>(std::difftime(std::time(nullptr), entryTime));
}

void ParkingSlot::occupy(const std::string& vid) {
    this->vehicleId = vid;
    this->statusIsFree = false;
    this->entryTime = std::time(nullptr);
}

void ParkingSlot::free() {
    this->vehicleId.clear();
    this->statusIsFree = true;
    this->entryTime = 0;
}

std::string ParkingSlot::toDisplayString() const {
    std::ostringstream oss;
    oss << "P" << std::setw(2) << std::setfill('0') << id << " [" 
        << (statusIsFree ? " " : "X") << "]";
    return oss.str();
}
