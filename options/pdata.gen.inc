struct Pdata {
std::unordered_map<std::string, int64_t> timings;
std::vector<cv::Vec4i> lanes;
cv::Mat camera_image;
cv::Mat analysis;
PressedKeys pressed_keys;
};
