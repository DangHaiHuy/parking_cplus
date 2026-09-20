#ifndef PARKING_SLOT_H
#define PARKING_SLOT_H

#include <string>
#include <ctime>
#include <chrono>
using namespace std;

enum class SlotStatus {
    FREE,
    OCCUPIED
};

class ParkingSlot {
public:
    ParkingSlot(int id);

    int getId() const;
    SlotStatus getStatus() const;
    bool isFree() const;
    const string& getVehicleId() const;
    time_t getEntryTime() const;
    long getParkedSeconds() const;

    bool occupy(const string& vehicleId);
    long release();

    string toDisplayString() const;

private:
    int id;
    SlotStatus status;
    string vehicleId;
    time_t entryTime;
};

#endif // PARKING_SLOT_H
