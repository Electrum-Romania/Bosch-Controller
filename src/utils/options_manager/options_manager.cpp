#include "options_manager.h"

OptionsManager::OptionsManager(CommandSocket &cs)
	: def(get_defs()), current(new Options)
{
	*current = *def;

#include "generated/options_register.gen.inc"
}

OptionsManager::~OptionsManager()
{
	delete current;
	delete def;
}

Options* OptionsManager::get_defs()
{
	auto* def = new Options;

#include "generated/options_defaults.gen.inc"

    return def;
}

Options* OptionsManager::get_options()
{
    std::lock_guard l(mux);

	Options* opt = new Options;

	*opt = *current;

	return opt;
}

void OptionsManager::parse_new_option_wrapper(void *t, const std::string& command, const std::string& arg)
{
    auto *options_manager = (OptionsManager *) t;

    options_manager->parse_new_option(command, arg);
}

void OptionsManager::parse_new_option(const std::string& command, const std::string& arg)
{
    std::lock_guard l(mux);

#include "generated/options_parse.gen.inc"

}
