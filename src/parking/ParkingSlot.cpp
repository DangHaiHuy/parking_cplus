#include "ParkingSlot.h"

#include <iomanip>
#include <sstream>

using namespace std;

ParkingSlot::ParkingSlot(int id) : id(id), statusIsFree(true), entryTime(0) {}

int ParkingSlot::getId(){
    return id;
}

bool ParkingSlot::isFree(){
    return statusIsFree;
}

string& ParkingSlot::getVehicleId(){
    return vehicleId;
}

time_t ParkingSlot::getEntryTime() {
    return entryTime;
}

long ParkingSlot::getParkedSeconds(){
    if (statusIsFree) return 0;
    return static_cast<long>(difftime(time(nullptr), entryTime));
}

void ParkingSlot::occupy(string& vid) {
    this->vehicleId = vid;
    this->statusIsFree = false;
    this->entryTime = time(nullptr);
}

void ParkingSlot::free() {
    this->vehicleId.clear();
    this->statusIsFree = true;
    this->entryTime = 0;
}

string ParkingSlot::toDisplayString(){
    string idStr = (id < 10 ? "0" : "") + to_string(id);
    string status = statusIsFree ? " " : "X";
    return "P" + idStr + " [" + status + "]";
}
