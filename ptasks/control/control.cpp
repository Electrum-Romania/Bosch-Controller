#include "control.h"

Control::Control()
    : Ptask("Control", 'c')
{

}

void Control::compute(Pdata *pdata, const Options *options)
{

}

void lane_correction(Pdata *pdata, const Options *options, double lane1_angle, double lane2_angle, int lane1_x_pos, int lane2_x_pos)
{
    double road_angle = (lane1_angle + lane2_angle) / 2;

    int car_x_pos = (lane1_x_pos + lane2_x_pos) / 2;
    int car_delta_x_pos = car_x_pos - options->control_steer_x_midpoint;

    double control_angle = road_angle + car_delta_x_pos * options->control_steer_kp;

    double control_command = control_angle / (options->control_steer_max_angle / options->control_steer_max_command);

    pdata->control_steer = control_command;
}
