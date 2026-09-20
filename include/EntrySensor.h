#ifndef ENTRY_SENSOR_H
#define ENTRY_SENSOR_H

#include <string>
#include <optional>

class EntrySensor
{
private:
    static bool isValidVehicleId(const std::string &id);

public:
    EntrySensor() = default;

    std::optional<std::string> detectVehicle(const std::string &rawVehicleId) const;
};

#endif // ENTRY_SENSOR_H