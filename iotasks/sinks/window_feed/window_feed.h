#ifndef IOTASKS_SINKS_WINDOW_FEED_H
#define IOTASKS_SINKS_WINDOW_FEED_H

#include "../../iotask.h"

#include <opencv2/opencv.hpp>

class WindowFeed : public IOtask {
private:
	cv::String window_name;

public:
	WindowFeed(const char *);

	void compute_frame() override;
};

#endif
