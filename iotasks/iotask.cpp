#include "iotask.h"

#include <cassert>

// using std::assert;

IOtask::IOtask(std::string name, int key)
	: busy(false), give_frame(0), done_frame(nullptr), pdata(nullptr), options(nullptr)
{
    screen_index = logger.request_screen(key, name);
    watch_index = logger.request_watch(key, std::move(name));
}

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
