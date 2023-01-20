#ifndef PTASKS_PTASK_H
#define PTASKS_PTASK_H

#include "../controller.h"

#include <string>

class Ptask {
private:
	std::string name;

public:
	Ptask(std::string name);

	virtual void compute(Pdata*, const Options*) = 0;

	void run(Pdata*, const Options*);

	~Ptask();
};

#endif
