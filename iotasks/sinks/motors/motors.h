//
// Created by grffn on 2/15/23.
//

#ifndef CONTROLLERPROJECT_MOTORS_H
#define CONTROLLERPROJECT_MOTORS_H

#include "../../iotask.h"
#include "../../../utils/serial/serial.h"

class Motors : public IOtask {
private:
    double current_motor_speed{};
    Serial &nucleo;

    int current_motor_speed_watch_value_index;

public:
    explicit Motors(Serial&);

    void compute_frame() override;
};


#endif //CONTROLLERPROJECT_MOTORS_H
