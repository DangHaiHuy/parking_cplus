#include "Barrier.h"
#include "Config.h"
#include "Logger.h"

#include <ctime>

using namespace std;

Barrier::Barrier(const string& name) : name(name), open(false) {}

void sleepMs(int ms) {
    clock_t start = clock();
    while ((clock() - start) * 1000 / CLOCKS_PER_SEC < ms);
}

void Barrier::raise() {
    if (open) return;
    Logger::getInstance().logBarrier(name, "OPENING...");
    sleepMs(Config::BARRIER_MOVEMENT_MS);
    open = true;
    Logger::getInstance().logBarrier(name, "OPENED");
}

void Barrier::lower() {
    if (!open) return;
    Logger::getInstance().logBarrier(name, "CLOSING...");
    sleepMs(Config::BARRIER_MOVEMENT_MS);
    open = false;
    Logger::getInstance().logBarrier(name, "CLOSED");
}

string Barrier::stateToString(){
    return open ? "OPEN" : "CLOSED";
}

bool Barrier::isOpen(){
    return open;
}
