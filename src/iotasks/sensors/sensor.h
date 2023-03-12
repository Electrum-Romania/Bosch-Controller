#ifndef IOTASKS_SINK_H
#define IOTASKS_SINK_H

#include <iotasks/iotask.h>

#include <utility>


///
/// \breif Base class for output tasks.
///
/// This class is identical to the IOtask class. Output IOtasks should inherit from this class for organizational
/// purpouses.
class Sink : public IOtask {
public:
    inline Sink(std::string name, int key, const std::vector<WatchPair>& s = {})
        : IOtask(std::move(name), key, s)
    {}

    ~Sink() override = default;
};

#endif //IOTASKS_SINK_H
