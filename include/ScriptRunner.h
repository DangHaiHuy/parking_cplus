#ifndef SCRIPT_RUNNER_H
#define SCRIPT_RUNNER_H

#include "AutoSensor.h"

#include <iosfwd>
#include <string>

bool runScript(std::istream &input, ParkingController &controller,
               AutoSensor &sensor, int intervalMs, std::ostream &errors);
bool parseNonNegativeInt(const std::string &value, int &result);

#endif
