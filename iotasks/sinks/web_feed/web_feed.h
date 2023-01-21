#ifndef IOTASKS_SINKS_WEBFEED_H
#define IOTASKS_SINKS_WEBFEED_H

#include "../../iotask.h"

#include <sockpp/tcp_connector.h>
#include <opencv2/opencv.hpp>

class WebFeed : public IOtask {
private:
	sockpp::tcp_connector conn;

	bool dead;
	
public:
	WebFeed(std::string host, uint16_t port);

	void compute_frame() override;
};

#endif
