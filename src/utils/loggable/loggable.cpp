#include <utils/loggable/loggable.h>

nclogger::Logger logger;

Loggable::Loggable(std::string name, int key, const std::vector<WatchPair>& s)
{
    if (!s.empty()) {
        watch_index = logger.request_watch(key, name);

        for (auto& watch_pair : s) {
            int value_index = logger.register_watch_value(watch_index, watch_pair.name, watch_pair.width);
            watch_index_map.insert({watch_pair.i, value_index});
            watch_width_map.insert({watch_pair.i, watch_pair.width});
        }
    }

    screen_index = logger.request_screen(key, std::move(name));
}

void Loggable::log(LogLevel log_level, std::string message) const
{
    logger.log(screen_index, log_level, std::move(message));
}

void Loggable::set_watch_value(int i, std::string value)
{
    logger.set_watch_value(watch_index, watch_index_map.at(i), std::move(value));
}

void Loggable::set_watch_value(int i, double d, int precision)
{
    logger.set_watch_value(watch_index, watch_index_map.at(i), nclogger::to_string(d, watch_width_map.at(i), [](auto &ss) {
        fixed(ss);
        ss.precision(2);
    }));
}

template<typename T>
void Loggable::set_watch_value(int i, T value, void (*callback)(std::stringstream& ss))
{
    if (callback)
        logger.set_watch_value(watch_index, watch_index_map.at(i), nclogger::to_string(value, watch_width_map.at(i), callback));
    else
        logger.set_watch_value(watch_index, watch_index_map.at(i), nclogger::to_string(value, watch_width_map.at(i)));
}
