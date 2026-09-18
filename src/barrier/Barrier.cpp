#include "Barrier.h"
#include "Config.h"
#include "Logger.h"

#include <chrono>
#include <thread>

Barrier::Barrier(const std::string& name) : name(name), open(false) {}

void Barrier::raise() {
    if (open) return;
    Logger::getInstance().logBarrier(name, "OPENING...");
    std::this_thread::sleep_for(std::chrono::milliseconds(Config::BARRIER_MOVEMENT_MS));
    open = true;
    Logger::getInstance().logBarrier(name, "OPENED");
}

void Barrier::lower() {
    if (!open) return;
    Logger::getInstance().logBarrier(name, "CLOSING...");
    std::this_thread::sleep_for(std::chrono::milliseconds(Config::BARRIER_MOVEMENT_MS));
    open = false;
    Logger::getInstance().logBarrier(name, "CLOSED");
}

std::string Barrier::stateToString() const {
    return open ? "OPEN" : "CLOSED";
}

bool Barrier::isOpen() const {
    return open;
}
