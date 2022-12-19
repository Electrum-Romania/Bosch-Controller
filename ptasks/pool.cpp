#include "pool.h"

PtaskPool::PtaskPool()
{
	int num_threads = std::thread::hardware_concurrency();
	worker_threads.resize(num_threads);

	for (int i = 0; i < num_threads; i++) {
		worker_threads.at(i) = std::thread(&PtaskPool::worker_loop, this);
	}
}

void PtaskPool::worker_loop()
{
	for (;;) {
		std::unique_lock<std::mutex> lock(queue_mutex);

		mutex_condition.wait(lock, [this] {
			return !jobs.empty();
		});

		Job job = jobs.front();
		jobs.pop();

		lock.unlock();

		job.ptask->compute(job.pdata, job.options);

		job.done.count_down();
	}
}

void PtaskPool::run_tasks(std::vector<Ptask*> tasks, Pdata* pdata, const Options* opt, std::latch& done)
{
	{
		std::unique_lock<std::mutex> lock(queue_mutex);

		for (auto t : tasks) {
			jobs.push( (Job) {
				.ptask = t,
				.pdata = pdata,
				.options = opt,
				.done = done,
			});
		}
	}

	for (int i = 0; i < tasks.size(); i++)
		mutex_condition.notify_one();
}

void PtaskPool::run_tasks_wait(std::vector<Ptask*> tasks, Pdata* pdata, const Options* opt)
{
	std::latch done{static_cast<std::ptrdiff_t>(tasks.size())};

	run_tasks(tasks, pdata, opt, done);

	done.wait();
}
