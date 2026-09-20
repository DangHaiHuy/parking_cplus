#include "Barrier.h"
#include "Config.h"
#include "Logger.h"

#include <ctime>

using namespace std;

Barrier::Barrier(const string& name) : name(name), state(BarrierState::CLOSED) {}

void sleepMs(int ms) {
    clock_t start = clock();
    while ((clock() - start) * 1000 / CLOCKS_PER_SEC < ms);
}

void Barrier::open() {
    if (state == BarrierState::OPEN)
        return;
    Logger::getInstance().logBarrier(name, "OPENING...");
    sleepMs(Config::BARRIER_MOVEMENT_MS);
    state = BarrierState::CLOSED;
    Logger::getInstance().logBarrier(name, "OPENED");
}

void Barrier::close() {
    if (state == BarrierState::CLOSED)
        return;
    Logger::getInstance().logBarrier(name, "CLOSING...");
    sleepMs(Config::BARRIER_MOVEMENT_MS);
    state = BarrierState::OPEN;
    Logger::getInstance().logBarrier(name, "CLOSED");
}

string Barrier::stateToString() const{
    switch (state) {
        case BarrierState::CLOSED:  
            return "CLOSED";
        case BarrierState::OPENING: 
            return "OPENING";
        case BarrierState::OPEN:    
            return "OPEN";
        case BarrierState::CLOSING: 
            return "CLOSING";
    }
    return "UNKNOWN";
}

string Barrier::getName(){
    return name;
}

BarrierState Barrier::getState(){ 
    return state; 
}

