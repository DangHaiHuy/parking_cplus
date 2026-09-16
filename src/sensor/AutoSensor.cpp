#include "AutoSensor.h"
#include "Display.h"
#include <algorithm>
#include <stdexcept>
#include <thread>

AutoSensor::AutoSensor(unsigned seed) : random(seed) {}

void AutoSensor::run(ParkingController &controller, int count, int intervalMs)
{
    if (count < 0 || intervalMs < 0)
        throw std::invalid_argument("Invalid auto configuration");
    for (int i = 0; i < count; ++i)
    {
        if (i > 0)
            std::this_thread::sleep_for(std::chrono::milliseconds(intervalMs));
        std::vector<std::string> parked;
        for (const auto &slot : controller.getSlots())
        {
            if (!slot.isFree())
                parked.push_back(slot.getVehicleId());
        }
        if (!parked.empty() && std::bernoulli_distribution(0.4)(random))
        {
            controller.handleVehicleExit(parked[std::uniform_int_distribution<std::size_t>(0, parked.size() - 1)(random)]);
        }
        else
        {
            std::string id;
            do
            {
                id = "AUTO-" + std::to_string(nextVehicle++);
            } while (std::find(parked.begin(), parked.end(), id) != parked.end());
            controller.handleVehicleEntry(id);
        }
        Display::showStatus(controller);
    }
}