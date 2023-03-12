#include <iotasks/sensors/camera/camera.h>

#include <iostream>

Camera::Camera()
	: Sensor("Camera", 'c'), camera(0)
{
	if (!camera.isOpened()) {
		log(LogLevel::FATAL, "Could not open camera feed!");
		std::exit(1);
	}
}

Camera::~Camera() = default;

void Camera::compute_frame()
{
	cv::Mat image;

	camera.read(image);

	if (image.empty()) {
        log(LogLevel::ERROR, "ERROR! blank frame grabbed");
		return;
	}

	pdata->camera_image = image;
}
