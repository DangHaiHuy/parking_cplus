#ifndef EXIT_SENSOR_H
#define EXIT_SENSOR_H

#include <string>
#include <optional>

class ExitSensor
{
public:
    ExitSensor() = default;

    std::optional<std::string> detectVehicle(const std::string &rawVehicleId) const;

private:
    static bool isValidVehicleId(const std::string &id);
};

#endif // EXIT_SENSOR_H