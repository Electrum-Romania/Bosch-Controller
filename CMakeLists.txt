cmake_minimum_required(VERSION 3.1...3.25)

project(
		ControllerProject
		VERSION 1.0
		LANGUAGES CXX
)

find_package(OpenCV REQUIRED)
find_package(nclogger REQUIRED)
find_package(Curses REQUIRED)

add_executable(
		controller

		main.cpp
		sched.cpp

		controller.h

		iotasks/iotask.cpp
		iotasks/iotask.h
		iotasks/sensors/camera/camera.cpp
		iotasks/sensors/camera/camera.h
		iotasks/sinks/window_feed/window_feed.cpp
		iotasks/sinks/window_feed/window_feed.h
		iotasks/sinks/web_feed/web_feed.cpp
		iotasks/sinks/web_feed/web_feed.h

		ptasks/ptask.cpp
		ptasks/ptask.h
		ptasks/pool.cpp
		ptasks/pool.h
		ptasks/control/control.cpp
		ptasks/control/control.h
		ptasks/lane_detection/lane_detection.cpp
		ptasks/lane_detection/lane_detection.h

		options/options_manager.cpp
		options/options_manager.h

		utils/serial/serial.cpp
		utils/serial/serial.h
		utils/command_socket/command_socket.cpp
		utils/command_socket/command_socket.h
		utils/command_socket/line_reader.cpp
		utils/command_socket/line_reader.h
		iotasks/sensors/keyboard/keyboard.cpp iotasks/sensors/keyboard/keyboard.h iotasks/sinks/motors/motors.cpp iotasks/sinks/motors/motors.h)

target_link_libraries(controller ${OpenCV_LIBS} sockpp nclogger::nclogger ${CURSES_LIBRARIES})
target_include_directories(controller PRIVATE ${CURSES_INCLUDE_DIR})

target_compile_features(controller PRIVATE cxx_std_20)

if (("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang") OR
	("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU"))

	add_compile_options("-Wall -Wextra -Wpedantic")

endif()
