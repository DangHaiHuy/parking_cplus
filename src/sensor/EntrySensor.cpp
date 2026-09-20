#include "EntrySensor.h"
#include <cctype>

bool EntrySensor::isValidVehicleId(const std::string &id)
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

    // Chi chap nhan chu, so, dau '-' va dau '_'.
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

std::optional<std::string> EntrySensor::detectVehicle(const std::string &rawVehicleId) const{
    if (!isValidVehicleId(rawVehicleId))
    {
        return std::nullopt;
    }

    return rawVehicleId;
}