#ifndef BARRIER_H
#define BARRIER_H

#include <string>

class Barrier {
public:
    Barrier(const std::string& name);

    void raise();
    void lower();

    std::string stateToString();
    bool isOpen();

private:
    std::string name;
    bool open;
};

#endif // BARRIER_H
