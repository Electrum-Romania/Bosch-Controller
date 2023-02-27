#include "lane_detection.h"

#include <chrono>
#include <opencv2/opencv.hpp>

// OBS input:  x: 1920
//		   y: 1080
// web output: x: 300
//		   y: 366


LaneDetection::LaneDetection() : Ptask("LaneDetection", 'l') {

}

cv::Mat warp_lane(cv::Mat src, Pdata* pdata, const Options* options) {

	cv::Point point1 = cv::Point(options->lane_perspective_startroof, src.size().height - options->lane_perspective_roof);
	cv::Point point2 = cv::Point(options-> lane_perspective_stoproof, src.size().height - options->lane_perspective_roof);
	cv::Point point3 = cv::Point(options->lane_perspective_startfloor, src.size().height - options->lane_perspective_floor);
	cv::Point point4 = cv::Point(options-> lane_perspective_stopfloor, src.size().height - options->lane_perspective_floor);

	cv::Point2f srcPoints[] = {
		point1,
		point2,
		point3,
		point4
	};

	std::vector<cv::Point> mask_points;

	mask_points.push_back(point1);
	mask_points.push_back(point2);
	mask_points.push_back(point3);
	mask_points.push_back(point4);

	pdata->lanes_perspective_mask = mask_points;

	cv::Point2f dstPoints[] = {
		cv::Point(0, 0),
		cv::Point(300, 0),
		cv::Point(0, 366),
		cv::Point(300, 366)
	};

	cv::Mat mat = cv::getPerspectiveTransform(srcPoints, dstPoints);

	cv::Mat dst;
	cv::warpPerspective(src, dst, mat, cv::Size(300, 366));

	return dst;

}

void LaneDetection::compute(Pdata* pdata, const Options* options)
{
	pdata->lanes_perspective = warp_lane(pdata->camera_image, pdata, options);
}

