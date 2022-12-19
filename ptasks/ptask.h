#ifndef PTASKS_PTASK_H
#define PTASKS_PTASK_H

#include "../controller.h"

class Ptask {
public:
	virtual void compute(Pdata*, const Options*) = 0;

	virtual ~Ptask() = 0;
};

#endif
