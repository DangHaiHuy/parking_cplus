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
        // Lan dau chay ngay. Cac lan sau moi doi theo intervalMs.
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
            // Tao ma xe moi va dam bao ma nay chua co trong bai.
            std::string vehicleId = "AUTO-" + std::to_string(nextVehicle);
            nextVehicle++;

            // Neu ma xe nay da co trong bai, tao ma moi cho den khi khong trung.
            while (std::find(parked.begin(), parked.end(), vehicleId) != parked.end())
            {
                vehicleId = "AUTO-" + std::to_string(nextVehicle);
                nextVehicle++;
            }

            controller.handleVehicleEntry(vehicleId);
        }

        Display::showStatus(controller);
    }
}