#ifndef IOTASKS_SINKS_WEBFEED_H
#define IOTASKS_SINKS_WEBFEED_H

#include "../../iotask.h"

#include <opencv2/opencv.hpp>

class WebFeed : public IOtask {
private:

public:
	WebFeed();

	void compute_frame() override;
};

#endif
