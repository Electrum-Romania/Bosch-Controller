#include <ptasks/control/control.h>

enum {
    watch_road_angle,
    watch_car_x,
    watch_car_delta_x,
    watch_control_angle,
    watch_control_command,
};

static
std::vector<Loggable::WatchPair> watch_pairs {
        {watch_road_angle, "Road Angle", 6},
        {watch_car_x, "Car X Position", 6},
        {watch_car_delta_x, "Car X Offset", 6},
        {watch_control_angle, "Control Angle", 6},
        {watch_control_command, "Control Command", 6},
};

Control::Control()
    : Ptask("Control", 'c', watch_pairs)
{
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

    set_watch_value(watch_road_angle, format_double(road_angle));
    set_watch_value(watch_car_x, format_int(car_x_pos));
    set_watch_value(watch_car_delta_x, format_int(car_delta_x_pos));
    set_watch_value(watch_control_angle, format_double(control_angle));
    set_watch_value(watch_control_command, format_double(control_command));

    pdata->control_steer = control_command;
}
