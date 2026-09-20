#ifndef BARRIER_H
#define BARRIER_H

#include <string>

enum class BarrierState {
    CLOSED,
    OPENING,
    OPEN,
    CLOSING
};

class Barrier {
public:
    Barrier(const std::string& name);

    void open();
    void close();

    BarrierState getState();
    std::string getName();
    std::string stateToString();

private:
    std::string name;
    BarrierState state;
};

#endif // BARRIER_H
