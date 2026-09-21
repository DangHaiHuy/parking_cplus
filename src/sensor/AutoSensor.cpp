#include "AutoSensor.h"
#include "Display.h"
#include <algorithm>
#include <chrono>
#include <stdexcept>
#include <thread>
#include <vector>

AutoSensor::AutoSensor(unsigned seed) : random(seed) {}

void AutoSensor::run(ParkingController &controller, int count, int intervalMs)
{
    if (count < 0 || intervalMs < 0)
    {
        throw std::invalid_argument("Invalid auto configuration");
    }

    for (int i = 0; i < count; ++i)
    {
        if (i > 0)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(intervalMs));
        }

        // Lay danh sach cac xe dang o trong bai.
        std::vector<std::string> parked;
        for (const auto &slot : controller.getSlots())
        {
            if (!slot.isFree())
            {
                parked.push_back(slot.getVehicleId());
            }
        }

        // Neu co xe trong bai, 40% kha nang chon mot xe de ra.
        bool shouldMakeVehicleExit = false; // Mac dinh la cho xe vao.
        if (!parked.empty())
        {
            std::bernoulli_distribution exitChance(0.4); // 40% true 60% false
            shouldMakeVehicleExit = exitChance(random);
        }

        if (shouldMakeVehicleExit)
        {
            std::uniform_int_distribution<std::size_t> chooseVehicle(0, parked.size() - 1); // sinh ngau nhien chi so (0, parked.size()-1)
            std::size_t vehicleIndex = chooseVehicle(random);
            std::string vehicleId = parked[vehicleIndex];
            controller.handleVehicleExit(vehicleId);
        }
        else
        {
            std::uint64_t plateSequence = nextVehicle - 1;
            char series = static_cast<char>('A' + (plateSequence / 99999) % 26);
            std::uint64_t serialNumber = plateSequence % 99999 + 1;
            std::string serial = std::to_string(serialNumber);
            serial.insert(0, 5 - serial.size(), '0');
            serial.insert(3, 1, '.');
            std::string vehicleId = "30" + std::string(1, series) + "-" + serial;
            nextVehicle++;

            while (std::find(parked.begin(), parked.end(), vehicleId) != parked.end())
            {
                plateSequence = nextVehicle - 1;
                series = static_cast<char>('A' + (plateSequence / 99999) % 26);
                serialNumber = plateSequence % 99999 + 1;
                serial = std::to_string(serialNumber);
                serial.insert(0, 5 - serial.size(), '0');
                serial.insert(3, 1, '.');
                vehicleId = "30" + std::string(1, series) + "-" + serial;
                nextVehicle++;
            }

            controller.handleVehicleEntry(vehicleId);
        }

        Display::showStatus(controller);
    }
}
