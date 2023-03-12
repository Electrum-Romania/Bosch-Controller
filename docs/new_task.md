\page new_task Creating a new task
\tableofcontents

This page provides a tutorial for creating a new task, and adding it to the `controller` runtime. 

# Introduction to tasks

Tasks represent the basic building block of the `controller` program. For a brief overview of the overall structure
of the program, please read the \ref design section first.

In short, tasks work with frame data. Sensors create frame data from external sources, Ptasks create processed frame
data from other frame data and Sinks output frame data to external sources. Frame data is stored in the `Pdata` data
structure.

# A basic task

A basic example of frame data is Pdata::camera_image which represents the image seen by the camera in the
current frame. A basic example of a Ptask would be one which takes the camera image and converts it to grayscale,
storing it in a new Pdata::grayscale_image slot.

## Creating the class

Firstly, decide on the name for your task class. Let us call our `Grayscale`. Create a directory in the
appropriate place: ptasks go in `src/ptasks` so we create `src/ptasks/grayscale` (sinks go in `src/iotasks/sinks` and
sensors go in `src/iotasks/sensor`). All the files related to this class go in this directory:

* `src/ptasks/grayscale/grayscale.h` - Class definition
* `src/ptasks/grayscale/grayscale.cpp` - Class implementation
* `src/ptasks/grayscale/pdata.inc` - Pdata generated (not present for Sinks)
* `src/ptasks/grayscale/options.inc` - Options used (optional)
* `src/ptasks/grayscale/static_options.inc` - StaticOptions used (optional)

\note `.h` and `.cpp` files MUST be added to `CMakeLists.txt`

A simple implementation of the Grayscale class would look as follows:

`src/ptasks/grayscale/grayscale.h`:

```
///
/// \file grayscale.h
/// \brief Grayscale class.

#ifndef PTASKS_GRAYSCALE_H
#define PTASKS_GRAYSCALE_H

#include <ptasks/ptask.h>

///
/// \brief Create grayscale image.
class Grayscale final : public Ptask {
public:
    Grayscale();
    void compute(Pdata*, const Options*) final;
};

#endif
```

`src/ptasks/grayscale/grayscale.cpp`:

```
#include <ptasks/grayscale/grayscale.h>

#include <opencv2/opencv.hpp>

Grayscale::Grayscale()
    // Name of the task and a unique key for the terminal log interface.
    : Ptask("Grayscale", 'g')
{}

void Grayscale::compute(Pdata *pdata, const Options *options)
{
    // This function gets called by the system for every frame.
    
    (void) options; // unused
    
    cv::cvtColor(pdata->camera_image, pdata->grayscale_image, CV_BGR2GRAY);
}
```

\note `generate_options.py` relies on `Pdata *` and `Options *` variables being named `pdata` and `options`, so please
keep that convention.

`src/ptasks/grayscale/pdata.inc`:

```
///
/// \brief Grayscaled version of the camera image.
cv::Mat grayscale_image;
```

## Adding the task to the list of tasks to be executed

The final step is to add the task to the list of tasks being executed. To do this, you must modify the `sched.cpp` file
first add the import to the header file at the beginning of the file:

```
#include <ptasks/grayscale/grayscale.cpp>
```

Then, search for the `ADD TASKS HERE` section. Add the following code:

```
ptasks_L1.push_back(new Grayscale());
```

## Using options

One of `controller`'s unique features are the hot-swappable Options parameters, which allow changing the functionality
of the program on-the-fly from the web interface. This is generally used in place of magic constants for algorithms.
To use options, you must include an `options.inc` file in the class directory. This file is similar to the `pdata.inc`
file with the exception that elements declared here must have a default value.

Excerpt from `src/ptasks/lane_detection/options.inc`:

```
std::uint8_t lane_detection_blur_level = 3;

short lane_detection_canny_thresh_low = 50;
short lane_detection_canny_thresh_high = 150;
```

In code, you will be able to access these options via the `const Options*` argument as `options->[option_name]`

## Using static_options

These are compile-time constants which can not be changed on the fly. To use them declare a file `static_options.inc`.
This file is similar to an `options.inc` file.

In code, you will be able to access these options as `StaticOptions::[option name]`.

\note Use hot-swappable options instead of static ones whenever you can! Generally, everything not needed at class
initialization time should be a hot-swappable option.

## Logging

The Ptask, Sink and Sensor classes inherit from the Loggable class, which provides protected functions for logging.
To log to the screen, write something like this in your class code:

```
log(LogLevel::INFO, "Hello, logs!");
```

You are encouraged to read the documentation for the Loggable which provides important information on logs, including
the real-time watch feature.

## Creating a sink or a sensor

The only difference between creating a Ptask as opposed to a Sink or a Sensor (Sinks and Sensors are `IOtask`s) is
the name of the main function you should override. As shown above, the function in Ptask is:

```
void compute(Pdata*, const Options*);
```

In IOtask, the function is

```
void compute_frame();
```

and `pdata` and `options` are available as protected members of the base class, but otherwise the functionality is the
same.
