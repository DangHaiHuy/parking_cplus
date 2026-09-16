#include "ExitSensor.h"
#include <algorithm>
#include <cctype>

bool ExitSensor::isValidVehicleId(const std::string &id)
{
    if (id.empty() || id.size() > 20)
    {
        return false;
    }
    return std::all_of(id.begin(), id.end(), [](unsigned char c)
                       { return std::isalnum(c) || c == '-' || c == '_'; });
}

std::optional<std::string> ExitSensor::detectVehicle(const std::string &rawVehicleId) const
{
    if (!isValidVehicleId(rawVehicleId))
    {
        return std::nullopt;
    }
    return rawVehicleId;
}