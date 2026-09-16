#ifndef SCRIPT_RUNNER_H
#define SCRIPT_RUNNER_H

#include "AutoSensor.h"

#include <iosfwd>
#include <string>

// Run the UTF-8 command script. Returns false only for a malformed command or
// an input-stream error; business-level rejections are handled by the controller.
bool runScript(std::istream &input, ParkingController &controller,
               AutoSensor &sensor, int intervalMs, std::ostream &errors);
bool parseNonNegativeInt(const std::string &value, int &result);

#endif // SCRIPT_RUNNER_H
