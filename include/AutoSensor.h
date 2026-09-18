#ifndef AUTO_SENSOR_H
#define AUTO_SENSOR_H

#include "ParkingController.h"
#include <cstdint>
#include <random>

class AutoSensor
{
public:
    explicit AutoSensor(unsigned seed = std::random_device{}());
    void run(ParkingController &controller, int count, int intervalMs);

private:
    std::mt19937 random;
    std::uint64_t nextVehicle = 1;
};

#endif