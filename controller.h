#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <unordered_map>
#include <opencv2/opencv.hpp>
#include <nclogger/logger.hpp>
#include <nclogger/to_string.hpp>

extern nclogger::Logger logger;
using nclogger::LogLevel;

struct PressedKeys {
    int w;
    int a;
    int s;
    int d;
};

#include "options/pdata.gen.inc"
#include "options/options.gen.inc"

#endif
