#include "options_manager.h"

OptionsManager::OptionsManager()
	: def(get_defs()), current(new Options)
{
	*current = *def;
}

OptionsManager::~OptionsManager()
{
	delete current;
	delete def;
}

#define DEFINE_OPTION_SETTING SET_DEFAULT
Options* OptionsManager::get_defs()
{
	Options* def = new Options;

#include "all_options.inc"

	return def;
}
#undef DEFINE_OPTIONS_SETTING

Options* OptionsManager::get_options()
{
	Options* opt = new Options;

	*opt = *current;

	return opt;
}
