#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <opencv2/opencv.hpp>

#define DEFINE_OPTION_DECLARE(T, DEC, VAL) T DEC;
#define DEFINE_OPTION_SET_DEFAULT(T, DEC, VAL) def->DEC = VAL;

#define DEFINE_OPTION3(T, DEC, VAL, SETTING) DEFINE_OPTION_##SETTING(T, DEC, VAL)
#define DEFINE_OPTION2(T, DEC, VAL, SETTING) DEFINE_OPTION3(T, DEC, VAL, SETTING)
#define DEFINE_OPTION(T, DEC, VAL) DEFINE_OPTION2(T, DEC, VAL, DEFINE_OPTION_SETTING)


struct Pdata {
#include "options/all_pdata.inc"
};


#define DEFINE_OPTION_SETTING DECLARE
struct Options {
#include "options/all_options.inc"
};
#undef DEFINE_OPTION_SETTING

#endif
