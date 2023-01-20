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

WebFeed::WebFeed(std::string host, uint16_t port)
	: conn({host, port}), dead(false)
{
	if (!conn) {
		std::cerr << "Error connecting to server: "
			<< conn.last_error_str() << std::endl;

		dead = true;

		return;
	}
}

void WebFeed::compute_frame()
{
	if (dead)
		return;

	std::vector<unsigned char> raw_image;

	cv::imencode(".jpg", pdata->camera_image, raw_image);

	uint32_t raw_image_len = raw_image.size();
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
		|| conn.write_n(raw_image.data(), raw_image.size()) == -1) {
		std::cerr << "Error writing data: "
			<< conn.last_error_str() << std::endl;

		dead = true;

		return;
	}

	std::cerr << "done sending image" << std::endl;
}
