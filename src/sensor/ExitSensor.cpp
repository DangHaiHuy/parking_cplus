#include "ExitSensor.h"
#include <cctype>
bool ExitSensor::isValidVehicleId(const std::string &id)
{
    // Ma xe phai co tu 1 den 20 ky tu.
    if (id.empty())
    {
        return false;
    }

    if (id.size() > 20)
    {
        return false;
    }

    // Kiem tra tung ky tu trong ma xe.
    for (char character : id)
    {
        bool isLetterOrNumber = std::isalnum(character);
        bool isDash = character == '-';
        bool isUnderscore = character == '_';

        if (!isLetterOrNumber && !isDash && !isUnderscore)
        {
            return false;
        }
    }

    return true;
}

std::optional<std::string> ExitSensor::detectVehicle(const std::string &rawVehicleId) const
{
    if (!isValidVehicleId(rawVehicleId))
    {
        return std::nullopt;
    }

    return rawVehicleId;
}