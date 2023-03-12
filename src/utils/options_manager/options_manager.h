#ifndef OPTIONS_MANAGER_H
#define OPTIONS_MANAGER_H

#include <controller.h>

#include <shared_mutex>

#include <utils/command_socket/command_socket.h>

class OptionsManager {
private:
	Options* def;
	Options* current;

    std::mutex mux;

	static Options* get_defs();

public:
	explicit OptionsManager(CommandSocket &cs);
	~OptionsManager();

    void parse_new_option(const std::string& command, const std::string& arg);
    static void parse_new_option_wrapper(void *t, const std::string& command, const std::string& arg);

	Options* get_options();
};

#endif
