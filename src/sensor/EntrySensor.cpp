#include "EntrySensor.h"
#include <algorithm>
#include <cctype>

bool EntrySensor::isValidVehicleId(const std::string &id)
{
    if (id.empty() || id.size() > 20)
    {
        return false;
    }
    // Chá»‰ cháº¥p nháº­n chá»¯, sá»‘ vÃ  dáº¥u gáº¡ch ngang/gáº¡ch dÆ°á»›i (vd: "51A-12345").
    return std::all_of(id.begin(), id.end(), [](unsigned char c)
                       { return std::isalnum(c) || c == '-' || c == '_'; });
}

std::optional<std::string> EntrySensor::detectVehicle(const std::string &rawVehicleId) const
{
    if (!isValidVehicleId(rawVehicleId))
    {
        return std::nullopt;
    }
    return rawVehicleId;
}