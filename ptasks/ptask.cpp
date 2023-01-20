#include <chrono>

#include "ptask.h"

namespace chrono = std::chrono;

Ptask::Ptask(std::string name)
    : name(name)
{}

void Ptask::run(Pdata* pdata, const Options* options)
{
    auto start = chrono::high_resolution_clock::now();

    this->compute(pdata, options);

    auto stop = chrono::high_resolution_clock::now();

	auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);

    pdata->timings[name] = duration.count();
}

Ptask::~Ptask() {}
