std::vector<cv::Point> lanes_perspective_mask;
std::vector<cv::Vec4i> lanes;
cv::Mat lanes_perspective;
cv::Mat lanes_inverted;
cv::Mat lanes_gray;
cv::Mat lanes_white;
cv::Mat lanes_blur;
cv::Mat lanes_dilate;
cv::Mat lanes_erode;
cv::Mat lanes_filled;
cv::Mat lanes_thresh;
cv::Mat lanes_window;
cv::Mat lanes_result;

double lane1_angle;
double lane2_angle;
int lane1_x_pos;
int lane2_x_pos;
