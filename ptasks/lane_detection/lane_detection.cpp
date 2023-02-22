#include "lane_detection.h"

#include <chrono>
#include <opencv2/opencv.hpp>

// OBS input:  x: 1920
//		   y: 1080
// web output: x: 300
//		   y: 366


LaneDetection::LaneDetection() : Ptask("LaneDetection", 'l') {

}


cv::Mat triangle_mask(cv::Size size, int p1_x, int p1_y, int p2_x, int p2_y, int p3_x, int p3_y)
{
	cv::Mat mat = cv::Mat::zeros(size, CV_8UC1);

	cv::Point pts[1][3];

	pts[0][0] = cv::Point(p1_x, p1_y);
	pts[0][1] = cv::Point(p2_x, p2_y);
	pts[0][2] = cv::Point(p3_x, p3_y);

	const cv::Point *ppt[1] = {pts[0]};

	int npts[] = {3};

	cv::fillPoly(mat, ppt, npts, 1, cv::Scalar(255));

	return mat;
}

cv::Mat warp_lane(cv::Mat src, Pdata* pdata, const Options* options) {
	cv::Point2f srcPoints[] = {
		cv::Point(options->lane_perspective_startroof, src.size().height - options->lane_perspective_roof),
		cv::Point(options-> lane_perspective_stoproof, src.size().height - options->lane_perspective_roof),
		cv::Point(options->lane_perspective_startfloor, src.size().height - options->lane_perspective_floor),
		cv::Point(options-> lane_perspective_stopfloor, src.size().height - options->lane_perspective_floor)
	};

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
	pdata->analysis = warp_lane(pdata->camera_image, pdata, options);
}

