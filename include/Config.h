#ifndef CONFIG_H
#define CONFIG_H

namespace Config {
constexpr int UTC_OFFSET_MINUTES = 7 * 60;
constexpr int AUTO_INTERVAL_MS = 500;
constexpr int BARRIER_MOVEMENT_MS = 1000;
constexpr int VEHICLE_PASSAGE_MS = 1000;
constexpr int DEFAULT_TOTAL_SLOTS = 10;
constexpr long MAX_PARKING_DURATION_SECONDS = 8 * 60 * 60;
}

#endif
