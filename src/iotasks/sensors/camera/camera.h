#ifndef IOTASKS_SENSORS_CAMERA_H
#define IOTASKS_SENSORS_CAMERA_H

#include "../../iotask.h"

#include <opencv2/opencv.hpp>

class Camera : public IOtask {
private:
//	cv::Mat          image;
	cv::VideoCapture camera;
	
public:
	Camera();
	~Camera();

	void compute_frame() override;
};

#endif
