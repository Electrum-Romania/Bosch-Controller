#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <unordered_map>
#include <opencv2/opencv.hpp>

struct PressedKeys {
    int w;
    int a;
    int s;
    int d;
};

#include "options/pdata.gen.inc"
#include "options/options.gen.inc"

#endif
