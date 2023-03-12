#ifndef CONTROLLER_CONTROL_HPP
#define CONTROLLER_CONTROL_HPP

#include "../ptask.h"

class Control : public Ptask {
private:
    int road_angle_watch_index;
    int car_x_pos_watch_index;
    int car_delta_x_watch_index;
    int control_angle_watch_index;
    int control_command_watch_index;

public:
    Control();
    void compute(Pdata*, const Options*) override;

    void lane_correction(Pdata *pdata, const Options *options);
};


#endif //CONTROLLER_CONTROL_HPP
