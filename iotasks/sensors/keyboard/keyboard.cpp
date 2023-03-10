//
// Created by grffn on 2/15/23.
//

#include "keyboard.h"

Keyboard::Keyboard(command_socket &cs)
    : pressed_keys({0, 0, 0, 0})
{
    cs.register_command("pressed_key", this, key_pressed_callback_wrapper);
}

void Keyboard::key_pressed_callback(const std::string &, const std::string &key)
{
    std::lock_guard l(mux);

    switch (key.at(0)) {
        case 'w': pressed_keys.w++; break;
        case 'a': pressed_keys.a++; break;
        case 's': pressed_keys.s++; break;
        case 'd': pressed_keys.d++; break;
    }
}

void Keyboard::key_pressed_callback_wrapper(void *t, const std::string &command, const std::string &key)
{
    auto *keyboard = (Keyboard *) t;
    keyboard->key_pressed_callback(command, key);
}

void Keyboard::compute_frame()
{
    std::lock_guard l(mux);

    pdata->pressed_keys = pressed_keys;
    pressed_keys = {0, 0, 0, 0};
}

Keyboard::~Keyboard() = default;
