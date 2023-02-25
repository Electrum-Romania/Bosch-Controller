#include "lane_detection.h"

#include <chrono>
#include <opencv2/opencv.hpp>

// OBS input:  x: 1920
//		   y: 1080
// web output: x: 300
//		   y: 366


LaneDetection::LaneDetection() : Ptask("LaneDetection") {

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
	mask_points.push_back(point4);
	mask_points.push_back(point3);

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

	cv::Mat inv;
	invert(mat, inv);
	pdata->lanes_inverted = inv;

	return dst;

}

cv::Mat extract_white(cv::Mat src, Pdata* pdata, const Options* options) {
	cv::Mat maskYellow, maskWhite;

	cv::inRange(src, cv::Scalar(20, 100, 100), cv::Scalar(30, 255, 255), maskYellow);
	cv::inRange(src, cv::Scalar(150, 150, 150), cv::Scalar(255, 255, 255), maskWhite);

	cv::Mat mask, dst;
	cv::bitwise_or(maskYellow, maskWhite, mask);
	cv::bitwise_and(src, mask, dst);	

	return dst;
}

cv::Mat fill_gaps(cv::Mat src, Pdata* pdata, const Options* options) {
	const cv::Size kernelSize = cv::Size(9, 9);
	cv::GaussianBlur(pdata->lanes_white, pdata->lanes_blur, kernelSize, 0);

	cv::Mat kernel = cv::Mat::ones(15, 15, CV_8U);
	cv::Mat dst;
	dilate(pdata->lanes_blur, pdata->lanes_dilate, kernel);
	erode(pdata->lanes_dilate, pdata->lanes_erode, kernel);
	morphologyEx(pdata->lanes_erode, dst, cv::MORPH_CLOSE, kernel);
	return dst;
}

std::vector<cv::Point2f> slidingWindow(cv::Mat image, cv::Rect window, Pdata* pdata)
{
	pdata->lanes_window = image;
    std::vector<cv::Point2f> points;
    const cv::Size imgSize = image.size();
    bool shouldBreak = false;
    int lastX = 0;

    while (true)
    {
		cv::rectangle(pdata->lanes_window, window, cv::Scalar(255, 255, 0));

        float currentX = window.x + window.width * 0.5f;
        
        cv::Mat roi= image(window); 
		std::vector<cv::Point2f> locations;
        
        cv::findNonZero(roi, locations);        
        float avgX = 0.0f;
        
        for (int i = 0; i < locations.size(); ++i) {
            float x = locations[i].x;
            avgX += window.x + x;
        }

        avgX = locations.empty() ? lastX : avgX / locations.size();
		lastX = avgX;
        
        cv::Point point(avgX, window.y + window.height * 0.5f);
        points.push_back(point);
        
        window.y -= window.height;
        
        if (window.y < 0)
        {
            window.y = 0;
            shouldBreak = true;
        }
        
        window.x += (point.x - currentX);
        
        if (window.x < 0)
            window.x = 0;
        if (window.x + window.width >= imgSize.width)
            window.x = imgSize.width - window.width - 1;
        
        if (shouldBreak)
            break;

		
    }
	
    return points;
}


void LaneDetection::compute(Pdata* pdata, const Options* options)
{
	pdata->lanes_perspective = warp_lane(pdata->camera_image, pdata, options);
	cv::cvtColor(pdata->lanes_perspective, pdata->lanes_gray, cv::COLOR_RGB2GRAY);	
	pdata->lanes_white = extract_white(pdata->lanes_gray, pdata, options);
	pdata->lanes_filled = fill_gaps(pdata->lanes_white, pdata, options);
	cv::threshold(pdata->lanes_filled, pdata->lanes_thresh, options->lane_threshold, 255, cv::THRESH_BINARY);

	std::vector<cv::Point2f> pts = slidingWindow(pdata->lanes_thresh, cv::Rect(options->lane_left_rect_x, options->lane_left_rect_y, options->lane_left_rect_width, options->lane_left_rect_height), pdata);
	std::vector<cv::Point> allPts; 
	std::vector<cv::Point2f> outPts;

	perspectiveTransform(pts, outPts, pdata->lanes_inverted);

	for (int i = 0; i < outPts.size() - 1; ++i) {
		cv::line(pdata->camera_image, outPts[i], outPts[i + 1], cv::Scalar(255, 0, 0), 3);
		allPts.push_back(cv::Point(outPts[i].x, outPts[i].y));
	}

	allPts.push_back(cv::Point(outPts[outPts.size() - 1].x, outPts[outPts.size() - 1].y));

	cv::Mat out;
	cv::cvtColor(pdata->lanes_thresh, out, cv::COLOR_GRAY2BGR);
	for (int i = 0; i < pts.size() - 1; ++i)    
		cv::line(out, pts[i], pts[i + 1], cv::Scalar(255, 0, 0));

	pts = slidingWindow(pdata->lanes_thresh, cv::Rect(options->lane_right_rect_x, options->lane_right_rect_y, options->lane_right_rect_width, options->lane_right_rect_height), pdata);
	cv::perspectiveTransform(pts, outPts, pdata->lanes_inverted);
	for (int i = 0; i < outPts.size() - 1; ++i)
	{
		cv::line(pdata->camera_image, outPts[i], outPts[i + 1], cv::Scalar(0, 0, 255), 3);
		allPts.push_back(cv::Point(outPts[outPts.size() - i - 1].x, outPts[outPts.size() - i - 1].y));
	}

	allPts.push_back(cv::Point(outPts[0].x - (outPts.size() - 1) , outPts[0].y));

	for (int i = 0; i < pts.size() - 1; ++i)
		cv::line(out, pts[i], pts[i + 1], cv::Scalar(0, 0, 255));

	std::vector<std::vector<cv::Point>> arr;
	arr.push_back(allPts);
	cv::Mat overlay = cv::Mat::zeros(pdata->camera_image.size(), pdata->camera_image.type());
	cv::fillPoly(overlay, arr, cv::Scalar(0, 255, 100));
	addWeighted(pdata->camera_image, 1, overlay, 0.5, 0, pdata->lanes_result);
}

