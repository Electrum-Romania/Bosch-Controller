#include "window_feed.h"

WindowFeed::WindowFeed(const char *window_name)
	: IOtask("WindowFeed", 'P'),
    window_name(window_name)
{}

void WindowFeed::compute_frame()
{
	cv::Mat frame = pdata->camera_image.clone();

	for( size_t i = 0; i < pdata->lanes.size(); i++ )
	{
		cv::line( frame, cv::Point(pdata->lanes[i][0], pdata->lanes[i][1]),
		cv::Point( pdata->lanes[i][2], pdata->lanes[i][3]), cv::Scalar(0,0,255), 3, 8 );
	}

	cv::imshow(window_name, frame);

	cv::waitKey(5);
}
