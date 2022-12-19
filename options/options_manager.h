#ifndef OPTIONS_MANAGER_H
#define OPTIONS_MANAGER_H

#include "../controller.h"

class OptionsManager {
private:
	Options* def;
	Options* current;

	static Options* get_defs();

public:
	OptionsManager();
	~OptionsManager();

	Options* get_options();
};

#endif
