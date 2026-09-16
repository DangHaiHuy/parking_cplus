#ifndef DISPLAY_H
#define DISPLAY_H

#include "ParkingController.h"

// Console-only presentation layer. Parking business rules remain in the controller.
class Display {
public:
    static void showStatus(const ParkingController &controller);
    static void showMenu();
};

#endif // DISPLAY_H
