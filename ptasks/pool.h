#ifndef PTASKS_POOL_H
#define PTASKS_POOL_H

#include "../controller.h"
#include "ptask.h"

#include <condition_variable>
#include <latch>
#include <mutex>
#include <thread>
#include <queue>
#include <vector>

class PtaskPool {
private:
	struct Job {
		Ptask*         ptask;
		Pdata*         pdata;
		const Options* options;
		std::latch&    done;
	};

	std::mutex              queue_mutex;
	std::condition_variable mutex_condition;
	std::queue<Job>   jobs;

	std::vector<std::thread> worker_threads;

	void worker_loop();
public:
	PtaskPool();

	void run_tasks(std::vector<Ptask*>, Pdata*, const Options*, std::latch&);
	void run_tasks_wait(std::vector<Ptask*> tasks, Pdata* pdata, const Options* opt);
};

#endif
