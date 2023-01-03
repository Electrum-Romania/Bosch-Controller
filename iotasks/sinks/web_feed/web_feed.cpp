#include "web_feed.h"

#include "httplib.h"

WebFeed::WebFeed(){}

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
