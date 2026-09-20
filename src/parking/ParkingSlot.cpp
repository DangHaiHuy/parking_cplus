#include "ParkingSlot.h"

#include <iomanip>
#include <sstream>

using namespace std;

ParkingSlot::ParkingSlot(int id) : id(id), status(SlotStatus::FREE), vehicleId(""), entryTime(0) {}

int ParkingSlot::getId() const {
    return id;
}

bool ParkingSlot::isFree() const {
    return status == SlotStatus::FREE;
}

const string& ParkingSlot::getVehicleId() const{
    return vehicleId;
}

SlotStatus ParkingSlot::getStatus() const{
    return status;
};


time_t ParkingSlot::getEntryTime() const {
    return entryTime;
}

long ParkingSlot::getParkedSeconds(){
    if (status == SlotStatus::FREE) return 0;
    return static_cast<long>(difftime(time(nullptr), entryTime));
}

bool ParkingSlot::occupy(string& vid) {
    if (!isFree()) return false;
    this->vehicleId = vid;
    this->status = SlotStatus::OCCUPIED;
    this->entryTime = time(nullptr);
}

long ParkingSlot::release() {
    if (status == SlotStatus::FREE) {
        return -1;
    }
    long parkedSeconds = getParkedSeconds();
    status = SlotStatus::FREE;
    vehicleId.clear();
    entryTime = 0;
    return parkedSeconds;
}

string ParkingSlot::toDisplayString() const{
    string idStr = (id < 10 ? "0" : "") + to_string(id);
    string stringStatus = (status == SlotStatus::FREE) ? " " : "X";
    return "P" + idStr + " [" + stringStatus + "]";
}
