#ifndef CONTROLLER_CONTROL_HPP
#define CONTROLLER_CONTROL_HPP

#include "../ptask.h"

class Control : public Ptask {
public:
    Control();
    void compute(Pdata*, const Options*) override;

    void lane_correction(Pdata *pdata, const Options *options, double lane1_angle, double lane2_angle, int lane1_x_pos, int lane2_x_pos);
};


#endif //CONTROLLER_CONTROL_HPP
