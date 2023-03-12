///
/// \file camera.h
/// \brief Camera class.

#ifndef IOTASKS_SENSORS_CAMERA_H
#define IOTASKS_SENSORS_CAMERA_H

#include <iotasks/sensors/sensor.h>

#include <opencv2/opencv.hpp>

///
/// \class Camera
/// \breif Camera class
class Camera : public Sensor {
private:
//	cv::Mat          image;
	cv::VideoCapture camera;
	
public:
	Camera();
	~Camera() override;

	void compute_frame() override;
};

#endif
