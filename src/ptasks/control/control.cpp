#include "control.h"

Control::Control()
    : Ptask("Control", 'c')
{
    road_angle_watch_index       = logger.register_watch_value(watch_index, "road angle", 6);
    car_x_pos_watch_index        = logger.register_watch_value(watch_index, "road angle", 6);
    car_delta_x_watch_index      = logger.register_watch_value(watch_index, "road angle", 6);
    control_angle_watch_index    = logger.register_watch_value(watch_index, "road angle", 6);
    control_command_watch_index  = logger.register_watch_value(watch_index, "road angle", 6);
}

static inline
std::string format_double(double d)
{
    return nclogger::to_string(d, 6, [](auto &ss) {
        std::fixed(ss);
        ss.precision(2);
    });
}

static inline
std::string format_int(int i)
{
    return nclogger::to_string(i, 6);
}


void Control::compute(Pdata *pdata, const Options *options)
{
    lane_correction(pdata, options);

}

void Control::lane_correction(Pdata *pdata, const Options *options)
{
    double road_angle = (pdata->lane1_angle + pdata->lane2_angle) / 2;
    int car_x_pos = (pdata->lane1_x_pos + pdata->lane2_x_pos) / 2;

    int car_delta_x_pos = car_x_pos - options->control_steer_x_midpoint;

    double control_angle = road_angle + car_delta_x_pos * options->control_steer_kp;

    double control_command = control_angle / (options->control_steer_max_angle / options->control_steer_max_command);

    logger.set_watch_value(watch_index, road_angle_watch_index, format_double(road_angle));
    logger.set_watch_value(watch_index, car_x_pos_watch_index, format_int(car_x_pos));
    logger.set_watch_value(watch_index, car_delta_x_watch_index, format_int(car_delta_x_pos));
    logger.set_watch_value(watch_index, control_angle_watch_index, format_double(control_angle));
    logger.set_watch_value(watch_index, control_command_watch_index, format_double(control_command));

    pdata->control_steer = control_command;
}
