#include "EntrySensor.h"
#include <cctype>

bool EntrySensor::isValidVehicleId(const std::string &id)
{
    if (id.size() < 8 || id.size() > 10)
    {
        return false;
    }

    // Hai chu so dau la ma tinh/thanh pho da duoc cap bien so.
    if (!std::isdigit(static_cast<unsigned char>(id[0])) ||
        !std::isdigit(static_cast<unsigned char>(id[1])))
    {
        return false;
    }

    int provinceCode = (id[0] - '0') * 10 + (id[1] - '0');
    bool isValidProvinceCode =
        (provinceCode >= 11 && provinceCode <= 12) ||
        (provinceCode >= 14 && provinceCode <= 38) ||
        provinceCode == 43 ||
        (provinceCode >= 47 && provinceCode <= 79) ||
        (provinceCode >= 81 && provinceCode <= 86) ||
        (provinceCode >= 88 && provinceCode <= 90) ||
        (provinceCode >= 92 && provinceCode <= 95) ||
        (provinceCode >= 97 && provinceCode <= 99);

    if (!isValidProvinceCode)
    {
        return false;
    }

    if (!std::isupper(static_cast<unsigned char>(id[2])) || id[3] != '-')
    {
        return false;
    }

    // Chap nhan 4 so cu, 5 so lien nhau hoac 5 so theo dang 123.45.
    if (id.size() == 10)
    {
        if (id[7] != '.')
        {
            return false;
        }

        for (std::size_t i = 4; i < id.size(); ++i)
        {
            if (i != 7 && !std::isdigit(static_cast<unsigned char>(id[i])))
            {
                return false;
            }
        }

        return true;
    }

    for (std::size_t i = 4; i < id.size(); ++i)
    {
        if (!std::isdigit(static_cast<unsigned char>(id[i])))
        {
            return false;
        }
    }

    return true;
}

std::optional<std::string> EntrySensor::detectVehicle(const std::string &rawVehicleId) const
{
    if (!isValidVehicleId(rawVehicleId))
    {
        return std::nullopt;
    }

    return rawVehicleId;
}
