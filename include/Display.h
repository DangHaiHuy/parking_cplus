#ifndef DISPLAY_H
#define DISPLAY_H

#include "ParkingController.h"

class Display {
public:
    static void showStatus(const ParkingController &controller);
    static void showMenu();
};

#endif
