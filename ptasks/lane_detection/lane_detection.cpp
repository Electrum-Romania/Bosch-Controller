#include "lane_detection.h"

#include <chrono>
#include <opencv2/opencv.hpp>


LaneDetection::LaneDetection() : Ptask("LaneDetection") {

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

void LaneDetection::compute(Pdata* pdata, const Options* options)
{
	cv::Mat grayscale_image;
	cv::cvtColor(pdata->camera_image, grayscale_image, cv::COLOR_BGR2GRAY);

	cv::Mat blur_image;
	cv::GaussianBlur(
		grayscale_image,
		blur_image,
		cv::Size(options->lane_detection_blur_level, options->lane_detection_blur_level),
		0
	);

	cv::Mat canny_image;
	cv::Canny(
		blur_image,
		canny_image,
		options->lane_detection_canny_thresh_low,
		options->lane_detection_canny_thresh_high
	);

	cv::Mat mask =
		triangle_mask(
			pdata->camera_image.size(),
			options->lane_detection_crop_x_start,
			pdata->camera_image.size().height - options->lane_detection_crop_floor,
			options->lane_detection_crop_x_stop,
			pdata->camera_image.size().height - options->lane_detection_crop_floor,
			options->lane_detection_crop_x_horizon,
			options->lane_detection_crop_roof
		);

	cv::Mat masked_image;
	cv::bitwise_and(canny_image, mask, masked_image);

	cv::HoughLinesP(masked_image, pdata->lanes, 1, CV_PI/180, 100, 50, 10);
}

