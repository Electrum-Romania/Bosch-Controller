#include "controller.h"

#include "iotasks/iotask.h"
#include "iotasks/sensors/camera/camera.h"
#include "iotasks/sinks/window_feed/window_feed.h"

#include "ptasks/ptask.h"
#include "ptasks/pool.h"
#include "ptasks/lane_detection/lane_detection.h"

#include "options/options_manager.h"

#include <chrono>
#include <thread>

namespace chrono = std::chrono;

static
std::vector<std::thread> launch_io(std::vector<IOtask *>& iotasks)
{
	std::vector<std::thread> threads;
	threads.reserve(iotasks.size());

	for (auto s : iotasks) {
		std::thread t([s] () {
				s->run();
			});

		threads.push_back(std::move(t));
	}

	return threads;
}

static
std::latch* call_io(std::vector<IOtask *>& iotasks, Pdata* pdata, const Options* opt)
{
	std::latch* l = new std::latch(iotasks.size());

	for (auto s : iotasks)
		s->frame(pdata, opt, l);

	return l;
}

void await_io(std::latch* l)
{
	l->wait();

	delete l;
}

void sched()
{
	OptionsManager opt_manager;

	PtaskPool ptask_pool;

	std::vector<IOtask*> sensors;
	std::vector<Ptask*> ptasks_L1;
	std::vector<IOtask*> sinks;

	sensors.push_back(new Camera());

	ptasks_L1.push_back(new LaneDetection());

	sinks.push_back(new WindowFeed("Feed"));

	std::vector<std::thread> sensor_threads = launch_io(sensors);
	std::vector<std::thread> sink_threads = launch_io(sinks);

	Pdata *pdata = new Pdata;
	Options *opt = opt_manager.get_options();

	std::latch* sensor_latch = call_io(sensors, pdata, opt);
	std::latch* sinks_latch = nullptr;

	Pdata   *old_pdata;
	Options *old_opt;
	bool first_frame = true;

	for (;;) {
		auto start = chrono::high_resolution_clock::now();

		Pdata *next_pdata = new Pdata;
		Options *next_opt = opt_manager.get_options();

		await_io(sensor_latch);

		sensor_latch = call_io(sensors, next_pdata, next_opt);

		ptask_pool.run_tasks_wait(ptasks_L1, pdata, opt);
//		ptask_pool.run_tasks_wait(ptasks_L2, pdata, opt);
//		ptask_pool.run_tasks_wait(ptasks_L3, pdata, opt);

		if (!first_frame) {
			await_io(sinks_latch);

			delete old_pdata;
			delete old_opt;
		} else {
			first_frame = false;
		}

		sinks_latch = call_io(sinks, pdata, opt);

		old_pdata = pdata;
		old_opt = opt;
		pdata = next_pdata;
		opt = next_opt;

		auto stop = chrono::high_resolution_clock::now();

		auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);

		std::cerr << "\033[1K\r" << duration;
	}
}
