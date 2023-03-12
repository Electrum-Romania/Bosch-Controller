#ifndef IOTASKS_IOTASK_H
#define IOTASKS_IOTASK_H

#include <controller.h>

#include <atomic>
#include <latch>
#include <semaphore>

#include <utils/loggable/loggable.h>

///
/// \brief Base class for input/output tasks.
///
/// This
/// \note Tasks should inherit from Sensor or Sink instead of inhereting from IOtask directly.
class IOtask : public Loggable {
private:
	std::binary_semaphore give_frame;
	std::latch*           done_frame = nullptr;
	std::atomic_bool      busy = false;

protected:
	Pdata*         pdata = nullptr;
	const Options* options = nullptr;

public:
    ///
    /// \brief Constructs a new IOtask.
    /// \param name
    /// \param key The key associated with the task in the interactive debugger.
    /// \param s
	IOtask(std::string name, int key, const std::vector<WatchPair>& s = {});
	virtual ~IOtask();

	virtual void compute_frame() = 0;

    [[noreturn]] void run();
	bool frame(Pdata*, const Options*, std::latch*);
};

#endif
