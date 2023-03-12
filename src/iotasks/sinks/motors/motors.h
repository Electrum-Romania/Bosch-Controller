#ifndef CONTROLLERPROJECT_MOTORS_H
#define CONTROLLERPROJECT_MOTORS_H

#include <iotasks/sinks/sink.h>

#include <iotasks/iotask.h>
#include <utils/serial/serial.h>

class Motors final : public Sink {
private:
    double current_motor_speed{};
    double current_ster{};
    Serial &nucleo;

public:
    explicit Motors(Serial&);
    ~Motors() final = default;

    void compute_frame() final;
};


#endif //CONTROLLERPROJECT_MOTORS_H
