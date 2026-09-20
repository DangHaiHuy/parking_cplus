#ifndef AUTO_SENSOR_H
#define AUTO_SENSOR_H

#include "ParkingController.h"
#include <cstdint>
#include <random>

class AutoSensor
{
private:
    std::mt19937 random;
    std::uint64_t nextVehicle = 1;

public:
    explicit AutoSensor(unsigned seed = std::random_device{}());
    void run(ParkingController &controller, int count, int intervalMs);
};

#endif