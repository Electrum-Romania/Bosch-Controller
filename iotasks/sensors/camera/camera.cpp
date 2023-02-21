#include "camera.h"

#include <chrono>
#include <iostream>
#include <thread>

Camera::Camera()
	: camera(2)
{
	//image = cv::imread("./test_image.jpg");

    //    // Check for failure
   //     if (image.empty()) {
    //            std::cerr << "Could not open or find the image" << std::endl;
	//	std::exit(1);
    //    }

	if (!camera.isOpened()) {
		std::cerr << "Could not open video feed" << std::endl;
		std::exit(1);
	}
}

Camera::~Camera() {}

void Camera::compute_frame()
{
	cv::Mat image;

	camera.read(image);

	if (image.empty()) {
		std::cerr << "ERROR! blank frame grabbed" << std::endl;
		return;
	}

	pdata->camera_image = image;
}
