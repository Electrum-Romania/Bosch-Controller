///
/// \file sensor.h
/// \brief Sensor class.

#ifndef IOTASKS_SENSOR_H
#define IOTASKS_SENSOR_H

#include <iotasks/iotask.h>

#include <utility>


///
/// \brief Base class for input tasks.
///
/// This class is identical to the IOtask class. Input IOtasks should inherit from this class for organizational
/// purposes.
class Sensor : public IOtask {
public:
    ///
    /// \brief Calls the IOtask constructor.
    ///
    /// \sa IOtask::IOtask()
    inline Sensor(std::string name, int key, const std::vector<WatchPair>& s = {})
        : IOtask(std::move(name), key, s)
    {}

    ~Sensor() override = default;
};

#endif //IOTASKS_SENSOR_H
