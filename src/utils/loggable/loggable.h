///
/// \file loggable.h
/// \brief Loggable class.

#ifndef UTILS_LOGGABLE_H
#define UTILS_LOGGABLE_H

#include <controller.h>

#include <unordered_map>

///
/// \brief Base class for logging classes.
///
/// This class provides a convenience abstraction layer over the nclogger library. It acquires a screen and a watch
/// and manages them. Logging should be done through this class's protected member functions.
///
/// To summarize the concepts of our ncurses logger: a _screen_ represents a series of logged messages generated by an
/// independent software component. The user can select to view the messages generated on a screen which will then
/// fill his whole terminal (hence the name "screen"), or can choose to view all of them at once.
///
/// A _watch_ is a collection of dynamic values which represent the current state of the system (currently active speed,
/// frame time, etc.). These are shown in ncurses in a side window which can be shown and hidden by the user. To use
/// watches in this project use the following idiom:
///
/// \code{.cpp}
///
/// enum {
///     watch_some_value,
///     watch_some_other_value,
/// };
///
/// static
/// std::vector<Loggable::WatchPair> watch_pairs {
///     {watch_some_value, "Some Value", 4}, // 4 is the width. should be appropriate for the value
///     {watch_some_other_value, "Some Other Value", 4},
/// };
///
/// MyLoggingClass::MyLoggingClass()
///     : Loggable("MyLoggingClass", 'm', watch_pairs) // 'm' is the key associated with this class.
///                                                    // should be unique for this class
/// {
///     // other initialization
/// }
///
/// MyLoggingClass::foo()
/// {
///     // some loop that happens perpetually
///     for (;;) {
///         T some_value = ...
///         T some_other_value = ...
///
///         // set watch values on every iteration
///         set_watch_value(watch_some_value, some_value);
///         set_watch_value(watch_some_other_value, some_other_value);
///
///         // only log if something unique happened
///         if (some_value < 0) {
///             log(LogLevel::Warn, "unexpected negative some_value");
///         }
///     }
/// }
///
/// \endcode
///
/// \note Almost all classes should inherit (directly or indirectly) from this class.
class Loggable {
public:
    ///
    /// \brief Description of a watch value.
    ///
    /// See the example code in the Loggable detailed description for more information on how this is used.
    struct WatchPair {
        ///
        /// \brief Value index later used in Loggable::set_watch_value().
        ///
        /// See the example code in the Loggable detailed description for more information on how this is used.
        int i;

        ///
        /// \brief The name of the value.
        ///
        /// This is shown on the screen to the user.
        std::string name;

        ///
        /// \brief The width of the value.
        ///
        /// This is how much space will be reserved on the screen for this value. If the provided string value is
        /// smaller than the width, it will be right-aligned. If it is smaller, the first width characters will be
        /// shown and the rest will be truncated.
        int width;
    };

private:
    int screen_index;
    int watch_index;

    std::unordered_map<int, int> watch_index_map;
    std::unordered_map<int, int> watch_width_map;

protected:
    ///
    /// \brief Constructs an nclogger screen and watch.
    ///
    /// \param name The name of the screen and watch. This is what will be seen by the user in the interface.
    /// \param key The key associated with the screen and watch. Should be unique.
    /// \param wp Description of values for the watch. If it is empty, a watch is not reserved at all.
    Loggable(std::string name, int key, const std::vector<WatchPair>& wp = {});

    ///
    /// \brief Log a message to the associated screen.
    ///
    /// \param log_level The log level of the message.
    /// \param message The text of the message.
    void log(LogLevel log_level, std::string message) const;

    ///
    /// \brief Sets a watch value.
    ///
    /// \param i The watch index as set in the WatchPair given to the constructor.
    /// \param value The text of the set message.
    void set_watch_value(int i, std::string value);

    ///
    /// \brief Sets a watch value from a double.
    ///
    /// The watch value will be set with the given double formatted with fixed precision.
    ///
    /// \param i The watch index as set in the WatchPair given to the constructor.
    /// \param d The double to use.
    /// \param precision The text of the set message.
    void set_watch_value(int i, double d, int precision = 2);

    ///
    /// \brief Sets a watch value from an arbitrary type.
    ///
    /// The watch value will be set with the given value, with formatting by stringstream.
    ///
    /// \param i The watch index as set in the WatchPair given to the constructor.
    /// \param value The value to be used.
    /// \param callback Function called before value is fed to ss. Can be used to set formatting parameters.
    template<typename T>
    void set_watch_value(int i, T value, void (*callback)(std::stringstream& ss) = nullptr);
};

#endif //UTILS_LOGGABLE_H