#ifndef PARKING_SLOT_H
#define PARKING_SLOT_H

#include <string>
#include <ctime>

using namespace std;

class ParkingSlot {
public:
    ParkingSlot(int id);

    int getId();
    bool isFree();
    string& getVehicleId();
    time_t getEntryTime();
    long getParkedSeconds();

    void occupy(string& vehicleId);
    void free();

    string toDisplayString();

private:
    int id;
    bool statusIsFree;
    string vehicleId;
    time_t entryTime;
};

#endif // PARKING_SLOT_H
