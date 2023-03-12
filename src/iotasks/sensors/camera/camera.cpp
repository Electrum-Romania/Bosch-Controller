#include "camera.h"

#include <chrono>
#include <iostream>
#include <thread>

Camera::Camera()
	: IOtask("Camera", 'c'), camera(0)
{
	//image = cv::imread("./test_image.jpg");

    //    // Check for failure
   //     if (image.empty()) {
    //            std::cerr << "Could not open or find the image" << std::endl;
	//	std::exit(1);
    //    }

	if (!camera.isOpened()) {
		logger.log(screen_index, LogLevel::FATAL, "Could not open camera feed!");
		std::exit(1);
	}
}

Camera::~Camera() = default;

void Camera::compute_frame()
{
	cv::Mat image;

	camera.read(image);

	if (image.empty()) {
        logger.log(screen_index, LogLevel::ERROR, "ERROR! blank frame grabbed");
		return;
	}

	pdata->camera_image = image;
}
