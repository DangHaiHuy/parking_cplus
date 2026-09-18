#ifndef PARKING_SLOT_H
#define PARKING_SLOT_H

#include <string>
#include <ctime>

class ParkingSlot {
public:
    explicit ParkingSlot(int id);

    int getId() const;
    bool isFree() const;
    const std::string& getVehicleId() const;
    time_t getEntryTime() const;
    long getParkedSeconds() const;

    void occupy(const std::string& vehicleId);
    void free();

    std::string toDisplayString() const;

private:
    int id;
    bool statusIsFree;
    std::string vehicleId;
    time_t entryTime;
};

#endif // PARKING_SLOT_H
