//
// Created by grffn on 2/15/23.
//

#ifndef CONTROLLERPROJECT_KEYBOARD_H
#define CONTROLLERPROJECT_KEYBOARD_H

#include "../../../controller.h"
#include "../../iotask.h"
#include "../../../utils/command_socket/command_socket.h"



class Keyboard : public IOtask {
private:
    std::mutex mux;
    PressedKeys pressed_keys;

public:
    Keyboard(command_socket &cs);
    ~Keyboard() override;

    void compute_frame() override;

    void key_pressed_callback(const std::string&, const std::string& key);
    static void key_pressed_callback_wrapper(void *t, const std::string&, const std::string& key);
};


#endif //CONTROLLERPROJECT_KEYBOARD_H
