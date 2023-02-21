#include "web_feed.h"

WebFeed::WebFeed(std::string host, uint16_t camera_port, uint16_t analysis_port)
	: camera_conn({host, camera_port}),
    analysis_conn({host, analysis_port}),
    dead(false)
{
	if (!camera_conn) {
		std::cerr << "Error connecting to server: "
			<< camera_conn.last_error_str() << std::endl;

		dead = true;

		return;
	}
    if (!analysis_conn) {
        std::cerr << "Error connecting to server: "
                  << analysis_conn.last_error_str() << std::endl;

        dead = true;

        return;
    }
}

void WebFeed::compute_frame()
{
	if (dead)
		return;

    write_image_to_connection(pdata->analysis1, camera_conn);


    write_image_to_connection(pdata->analysis2, analysis_conn);
}

void WebFeed::write_image_to_connection(cv::Mat& image, sockpp::tcp_connector &conn)
{
    cv::Mat resized_image;
    cv::resize(image, resized_image, cv::Size(300, 366), cv::INTER_LINEAR);

    std::vector<unsigned char> jpeg;

    cv::imencode(".jpg", resized_image, jpeg);

    write_jpeg_to_connection(jpeg, conn);
}

void WebFeed::write_jpeg_to_connection(std::vector<unsigned char>& jpeg, sockpp::tcp_connector &conn)
{
    uint32_t raw_image_len = jpeg.size();
    uint8_t  raw_image_len_encoded[4] = {
            static_cast<uint8_t>((raw_image_len >> 24) & 0xff),
            static_cast<uint8_t>((raw_image_len >> 16) & 0xff),
            static_cast<uint8_t>((raw_image_len >>  8) & 0xff),
            static_cast<uint8_t>((raw_image_len      ) & 0xff)
    };

    if (conn.write_n(raw_image_len_encoded, 4) == -1
        || conn.write_n(jpeg.data(), jpeg.size()) == -1) {
        std::cerr << "Error writing data: "
                  << conn.last_error_str() << std::endl;

        dead = true;

        return;
    }
}
