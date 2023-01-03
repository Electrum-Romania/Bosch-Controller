DEFINE_OPTION(std::string, web_feed_post_server,         "192.168.59.35:8000");
DEFINE_OPTION(std::string, web_feed_post_raw_image_path, "/updateImage/");
DEFINE_OPTION(int, lane_detection_crop_floor, 0);
DEFINE_OPTION(int, lane_detection_crop_roof, 250);
DEFINE_OPTION(int, lane_detection_crop_x_start, 200);
DEFINE_OPTION(int, lane_detection_crop_x_stop, 1100);
DEFINE_OPTION(int, lane_detection_crop_x_horizon, 550);

DEFINE_OPTION(uint8_t, lane_detection_blur_level, 3);

DEFINE_OPTION(short, lane_detection_canny_thresh_low, 50);
DEFINE_OPTION(short, lane_detection_canny_thresh_high, 150);
// DEFINE_OPTION(short, lane_detection_canny_sobel_size, );
