#include <chrono>
#include <utility>

#include "ptask.h"

namespace chrono = std::chrono;

Ptask::Ptask(std::string name)
    : name(std::move(name))
{}

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

    std::cerr << "\033[2J\033[J";
    std::cerr << "Timings\n";

    for (auto const& [task, time]: pdata->timings) {
        std::cerr << task << ":" << std::setw(3) << time << "ms\n";
    }

    std::cerr << "Total: " << total_time << "ms\n";
    #endif
}
