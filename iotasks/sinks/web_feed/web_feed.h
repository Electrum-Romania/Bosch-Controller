#ifndef IOTASKS_SINKS_WEBFEED_H
#define IOTASKS_SINKS_WEBFEED_H

#include "../../iotask.h"

#include <sockpp/tcp_connector.h>
#include <opencv2/opencv.hpp>

class WebFeed : public IOtask {
private:
	sockpp::tcp_connector camera_conn;
    sockpp::tcp_connector analysis_conn;


    bool dead;

    void write_jpeg_to_connection(std::vector<unsigned char>& jpeg, sockpp::tcp_connector& connector);
    void write_image_to_connection(cv::Mat &image, sockpp::tcp_connector &conn);
	
public:
	WebFeed(std::string host, uint16_t camera_port, uint16_t analysis_port);

	void compute_frame() override;

};

#endif
