#ifndef IOTASKS_IOTASK_H
#define IOTASKS_IOTASK_H

#include "../controller.h"

#include <atomic>
#include <latch>
#include <semaphore>

class IOtask {
private:
	std::binary_semaphore give_frame;
	std::latch*           done_frame;
	std::atomic_bool      busy;

protected:
	Pdata*         pdata;
	const Options* options;

public:
	IOtask();
	virtual ~IOtask();

	virtual void compute_frame() = 0;

	void run();
	bool frame(Pdata*, const Options*, std::latch*);
};

#endif
