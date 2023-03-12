#include <iotasks/iotask.h>

#include <cassert>

IOtask::IOtask(std::string name, int key, const std::vector<WatchPair>& s)
	: Loggable(std::move(name), key, s), give_frame(0)
{}

IOtask::~IOtask() = default;

[[noreturn]]
void IOtask::run()
{
	for (;;) {
		give_frame.acquire();

		compute_frame(); // call to child class

		busy = false;

		done_frame->count_down();
	}
}

bool IOtask::frame(Pdata* pdata, const Options* options, std::latch* done_frame)
{
	if (busy) {
		assert(false);
		return false;
	}

	busy = true;

	this->pdata = pdata;
	this->options = options;
	this->done_frame = done_frame;

	give_frame.release();

	return true;
}
