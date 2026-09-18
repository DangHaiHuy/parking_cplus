#ifndef BARRIER_H
#define BARRIER_H

#include <string>

class Barrier {
public:
    explicit Barrier(const std::string& name);

    void raise();
    void lower();

    std::string stateToString() const;
    bool isOpen() const;

private:
    std::string name;
    bool open;
};

#endif // BARRIER_H
