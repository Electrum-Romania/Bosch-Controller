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

		src/main.cpp
		src/sched.cpp

		src/controller.h

		src/iotasks/iotask.cpp
		src/iotasks/iotask.h
		src/iotasks/sensors/camera/camera.cpp
		src/iotasks/sensors/camera/camera.h
		src/iotasks/sinks/web_feed/web_feed.cpp
		src/iotasks/sinks/web_feed/web_feed.h

		src/ptasks/ptask.cpp
		src/ptasks/ptask.h
		src/ptasks/pool.cpp
		src/ptasks/pool.h
		src/ptasks/control/control.cpp
		src/ptasks/control/control.h
		src/ptasks/lane_detection/lane_detection.cpp
		src/ptasks/lane_detection/lane_detection.h
		src/ptasks/analysis/analysis.cpp
		src/ptasks/analysis/analysis.h

		src/utils/options_manager/options_manager.cpp
		src/utils/options_manager/options_manager.h

		src/utils/serial/serial.cpp
		src/utils/serial/serial.h
		src/utils/command_socket/command_socket.cpp
		src/utils/command_socket/command_socket.h
		src/utils/command_socket/line_reader.cpp
		src/utils/command_socket/line_reader.h
		src/iotasks/sensors/keyboard/keyboard.cpp src/iotasks/sensors/keyboard/keyboard.h src/iotasks/sinks/motors/motors.cpp src/iotasks/sinks/motors/motors.h src/utils/loggable/loggable.cpp src/utils/loggable/loggable.h src/iotasks/sinks/sink.h src/iotasks/sinks/sink.h src/iotasks/sensors/sensor.h)

target_link_libraries(controller ${OpenCV_LIBS} sockpp nclogger::nclogger ${CURSES_LIBRARIES})
target_include_directories(controller PRIVATE ${CURSES_INCLUDE_DIR} src/)

target_compile_features(controller PRIVATE cxx_std_20)

if (("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang") OR
	("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU"))

	add_compile_options("-Wall -Wextra -Wpedantic")

endif()
