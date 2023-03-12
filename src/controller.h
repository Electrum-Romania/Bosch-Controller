///
/// \file controller.h
/// \brief Main concepts used everywhere in the application.
///
/// \sa Pdata, Options

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <unordered_map>

#include <nclogger/logger.hpp>
#include <nclogger/to_string.hpp>
#include <opencv2/opencv.hpp>

using nclogger::LogLevel;

///
/// \brief Keys pressed on the keyboard.
///
/// Represents the keystrokes generated on the web interface since the last frame.
/// \sa Keyboard
struct PressedKeys {
    ///
    /// \brief Number of times the W key was pressed since last frame.
    int w;

    ///
    /// \brief Number of times the A key was pressed since last frame.
    int a;

    ///
    /// \brief Number of times the S key was pressed since last frame.
    int s;

    ///
    /// \brief Number of times the D key was pressed since last frame.
    int d;
};

///
/// \brief Main program loop.
///
/// This function runs continuously, processing frame. The process is as follows:
///
/// 1. Get current Options.
/// 2. Await for all sensors to give available data.
/// 3. Run layer 1 patsks in parallel. Await for all of them to complete.
/// 4. Run layer 2 ptasks in parallel. Await for all of them to complete. (and so on for other layers)
/// 5. Await for sinks to finish sending the _previous_ frame. Then, asynchronously give them access to the
/// current frame data.
///
/// Sensors are: Camera, Keyboard.
///
/// Layer 1 ptasks are: LaneDetection.
///
/// Layer 2 ptasks are: Analysis, Control.
///
/// Sinks are: Motors, WebFeed.
///
/// \sa \ref design, IOtask, Ptask
[[noreturn]] void sched();

#include <generated/pdata.gen.h>
#include <generated/options.gen.h>
#include <generated/static_options.gen.h>

#endif
