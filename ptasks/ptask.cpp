#include <chrono>
#include <utility>

#include "ptask.h"

namespace chrono = std::chrono;

int Ptask::timings_screen = -1;
std::unordered_map<std::string, int> Ptask::title_to_watch_value;

Ptask::Ptask(std::string name, int key)
    : name(name)
{
    screen_index = logger.request_screen(key, name);
    watch_index = logger.request_watch(key, std::move(name));
}

Ptask::~Ptask() = default;

void Ptask::run(Pdata* pdata, const Options* options)
{
    auto start = chrono::high_resolution_clock::now();

    this->compute(pdata, options);

    auto stop = chrono::high_resolution_clock::now();

	auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);

    pdata->timings[name] = duration.count();
}


void Ptask::print_timings(Pdata *pdata, int64_t total_time) {
    #if 0
    thread_local int i = 0;

    if (i++ % 10 != 0)
        return;

    if (timings_screen == -1) {
        timings_screen = logger.request_watch('t', "Ptask Timings");

        for (auto const& [task_title, time]: pdata->timings) {
            title_to_watch_value[task_title] = logger.register_watch_value(timings_screen, task_title, 5);
        }

        title_to_watch_value["total"] = logger.register_watch_value(timings_screen, "Total", 5);
    }

    long total = 0;

    for (auto const& [task_title, time]: pdata->timings) {
        total += time;
        logger.set_watch_value(timings_screen, title_to_watch_value[task_title], nclogger::to_string(time, 5));
    }

    logger.set_watch_value(timings_screen, title_to_watch_value["total"], nclogger::to_string(total, 5));
    std::cerr << "Total: " << total_time << "ms\n";
    #endif
}
