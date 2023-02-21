struct Options {
int lane_detection_crop_floor;
int lane_detection_crop_roof;
int lane_detection_crop_x_start;
int lane_detection_crop_x_stop;
int lane_detection_crop_x_horizon;
uint8_t lane_detection_blur_level;
short lane_detection_canny_thresh_low;
short lane_detection_canny_thresh_high;
int lane_perspective_floor;
int lane_perspective_roof;
int lane_perspective_startfloor;
int lane_perspective_stopfloor;
int lane_perspective_startroof;
int lane_perspective_stoproof;
std::string web_feed_post_server;
std::string web_feed_post_raw_image_path;
};
