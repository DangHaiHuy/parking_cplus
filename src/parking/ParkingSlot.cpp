#include "ParkingSlot.h"

#include <iomanip>
#include <sstream>

using namespace std;

ParkingSlot::ParkingSlot(int id) : id(id), status(SlotStatus::FREE), vehicleId(""), entryTime(0) {}

int ParkingSlot::getId(){
    return id;
}

bool ParkingSlot::isFree(){
    return status == SlotStatus::FREE;
}

string& ParkingSlot::getVehicleId(){
    return vehicleId;
}

time_t ParkingSlot::getEntryTime() {
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

string ParkingSlot::toDisplayString(){
    string idStr = (id < 10 ? "0" : "") + to_string(id);
    string stringStatus = (status == SlotStatus::FREE) ? " " : "X";
    return "P" + idStr + " [" + stringStatus + "]";
}
