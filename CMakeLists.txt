cmake_minimum_required(VERSION 3.1...3.25)

project(
	ControllerProject
	VERSION 1.0
	LANGUAGES CXX
)

find_package(OpenCV REQUIRED)

add_executable(
	controller

	main.cpp
	sched.cpp

	iotasks/iotask.cpp
	iotasks/sensors/camera/camera.cpp
	iotasks/sinks/window_feed/window_feed.cpp
	iotasks/sinks/web_feed/web_feed.cpp

	ptasks/ptask.cpp
	ptasks/pool.cpp
	ptasks/lane_detection/lane_detection.cpp

	options/options_manager.cpp
)

message(STATUS "Using library for SocketCAN samples: ${OpenCV_LIBS}")

target_link_libraries(controller ${OpenCV_LIBS} sockpp)

target_compile_features(controller PRIVATE cxx_std_20)

if (("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang") OR
	("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU"))

	add_compile_options("-Wall -Wextra -Wpedantic")

endif()
