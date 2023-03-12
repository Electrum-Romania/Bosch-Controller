#ifndef PTASKS_PTASK_H
#define PTASKS_PTASK_H

#include <controller.h>

#include <string>

#include <utils/loggable/loggable.h>

class Ptask : public Loggable {
private:
	std::string name;

    static int timings_screen;
    static std::unordered_map<std::string, int> title_to_watch_value;

public:
	explicit Ptask(std::string name, int key, std::vector<Loggable::WatchPair> wp = {});

    ~Ptask();

	virtual void compute(Pdata*, const Options*) = 0;
	void run(Pdata*, const Options*);

    static void print_timings(Pdata*, int64_t);
};

#endif
