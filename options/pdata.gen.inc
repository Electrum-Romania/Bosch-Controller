struct Pdata {
std::unordered_map<std::string, int64_t> timings;
cv::Mat analysis1;
cv::Mat analysis2;
std::vector<cv::Point> lanes_perspective_mask;
std::vector<cv::Vec4i> lanes;
cv::Mat lanes_perspective;
cv::Mat camera_image;
cv::Mat analysis;
PressedKeys pressed_keys;
};
