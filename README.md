Raspberry Pi controller
============

The controller program is Electrum's solution to control the car in the Bosch Future Mobility Challenge. It is a C++
application built from the ground up, based on what we call a frame system. It uses the OpenCV and sockpp libraries.
It is set apart by its ability to communicate with our web debugger which allow it to change its parameters on the fly.

Design {#design}
------

The structure of the `controller` is based around the concepts of
frames and tasks. A frame represents a set of input data (an image
from the camera feed, data from the proximity sensors) as well as all
the processing done on that data (such as the lanes or traffic
signs detected in the image). All the data related to a frame is
stored in a structure called `Pdata`. The process of computing a frame
is the process of filling the `Pdata` structure with raw and
processed data, then outputing the relevant information from the
structure to the Nucleo board and other ouputs.

Tasks are classes that work with the `Pdata` structure. There are
`IOtask`s (sensors and sinks) and `Ptask`s. Sensors fill `Pdata`
with data from external sources (eg the camera video feed).
`Ptask`s fill `Pdata` by processing already computed data (eg
traffic sign recognition). Finally, sinks take the fully filled
`Pdata` and output parts of it to consumers outside the program
(eg Nucleo board, camera feed, debug information).

`controller` is a concurrent, multithreaded program. Each `IOtask`
runs in its own thread and all `Ptask`s run in a thread pool taking
full advantage of the number of cpu process cores. Tasks are part of
a group called a layer. All tasks in a layer run in parallel. After
all tasks in a layer have finished running, the tasks in the next
layer are run, and so on. The sensors and sinks form the first and
last layers of the sequence, and the `Ptask`s are grouped in layers
numbered from 1 to n.
