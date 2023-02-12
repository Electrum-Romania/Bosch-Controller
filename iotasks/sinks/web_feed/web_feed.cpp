#include "web_feed.h"


#if 0
WebFeed::WebFeed()
{}

void WebFeed::compute_frame()
{
	std::vector<unsigned char> raw_image;

	cv::imencode(".jpg", pdata->camera_image, raw_image);

	httplib::Client cli(options->web_feed_post_server);

	httplib::Result result = cli.Post(
		options->web_feed_post_raw_image_path,
		(char *) raw_image.data(),
		raw_image.size(),
		"image/jpeg"
	);

	if (result)
		std::cerr << "Image posted" << std::endl;
	else
		std::cerr << "Could not post" << std::endl;
}
#endif

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

    write_image_to_connection(pdata->camera_image, camera_conn);

    cv::Mat analysis_frame = pdata->camera_image.clone();

    // Lanes
    for(size_t i = 0; i < pdata->lanes.size(); i++)
    {
        cv::line( analysis_frame, cv::Point(pdata->lanes[i][0], pdata->lanes[i][1]),
                  cv::Point( pdata->lanes[i][2], pdata->lanes[i][3]), cv::Scalar(0,0,255), 3, 8 );
    }

    write_image_to_connection(analysis_frame, analysis_conn);
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

    std::cerr << "ready to send image of size " << raw_image_len << std::endl;

    printf("0x%02x 0x%02x 0x%02x 0x%02x\n", raw_image_len_encoded[0],
           raw_image_len_encoded[1],
           raw_image_len_encoded[2],
           raw_image_len_encoded[3]);

    if (conn.write_n(raw_image_len_encoded, 4) == -1
        || conn.write_n(jpeg.data(), jpeg.size()) == -1) {
        std::cerr << "Error writing data: "
                  << conn.last_error_str() << std::endl;

        dead = true;

        return;
    }

    std::cerr << "done sending image" << std::endl;
}
