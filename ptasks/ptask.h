#ifndef PTASKS_PTASK_H
#define PTASKS_PTASK_H

#include "../controller.h"

#include <string>

class Ptask {
private:
	std::string name;

public:
	explicit Ptask(std::string name);

    ~Ptask();

	virtual void compute(Pdata*, const Options*) = 0;
	void run(Pdata*, const Options*);

    static void print_timings(Pdata*, int64_t);
};

#endif
