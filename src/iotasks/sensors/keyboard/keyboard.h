#ifndef CONTROLLERPROJECT_KEYBOARD_H
#define CONTROLLERPROJECT_KEYBOARD_H

#include <controller.h>

#include <iotasks/sensors/sensor.h>
#include <utils/command_socket/command_socket.h>

class Keyboard : public Sensor {
private:
    std::mutex mux;
    PressedKeys pressed_keys;

public:
    explicit Keyboard(CommandSocket &cs);
    ~Keyboard() override;

    void compute_frame() override;

    void key_pressed_callback(const std::string&, const std::string& key);
    static void key_pressed_callback_wrapper(void *t, const std::string&, const std::string& key);
};


#endif //CONTROLLERPROJECT_KEYBOARD_H
