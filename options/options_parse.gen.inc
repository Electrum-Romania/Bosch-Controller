if (command == "lane_detection_crop_floor") {
try {
current->lane_detection_crop_floor = std::stoi(arg);
} catch (const std::invalid_argument& e) {
std::cerr << "Invalid argument when parsing lane_detection_crop_floor: " << e.what() << std::endl;
} catch (const std::out_of_range& e) {
std::cerr << "Out of range when parsing lane_detection_crop_floor: " << e.what() << std::endl;
}
}
if (command == "lane_detection_crop_roof") {
try {
current->lane_detection_crop_roof = std::stoi(arg);
} catch (const std::invalid_argument& e) {
std::cerr << "Invalid argument when parsing lane_detection_crop_roof: " << e.what() << std::endl;
} catch (const std::out_of_range& e) {
std::cerr << "Out of range when parsing lane_detection_crop_roof: " << e.what() << std::endl;
}
}
if (command == "lane_detection_crop_x_start") {
try {
current->lane_detection_crop_x_start = std::stoi(arg);
} catch (const std::invalid_argument& e) {
std::cerr << "Invalid argument when parsing lane_detection_crop_x_start: " << e.what() << std::endl;
} catch (const std::out_of_range& e) {
std::cerr << "Out of range when parsing lane_detection_crop_x_start: " << e.what() << std::endl;
}
}
if (command == "lane_detection_crop_x_stop") {
try {
current->lane_detection_crop_x_stop = std::stoi(arg);
} catch (const std::invalid_argument& e) {
std::cerr << "Invalid argument when parsing lane_detection_crop_x_stop: " << e.what() << std::endl;
} catch (const std::out_of_range& e) {
std::cerr << "Out of range when parsing lane_detection_crop_x_stop: " << e.what() << std::endl;
}
}
if (command == "lane_detection_crop_x_horizon") {
try {
current->lane_detection_crop_x_horizon = std::stoi(arg);
} catch (const std::invalid_argument& e) {
std::cerr << "Invalid argument when parsing lane_detection_crop_x_horizon: " << e.what() << std::endl;
} catch (const std::out_of_range& e) {
std::cerr << "Out of range when parsing lane_detection_crop_x_horizon: " << e.what() << std::endl;
}
}
if (command == "lane_detection_blur_level") {
try {
current->lane_detection_blur_level = std::stoi(arg);
} catch (const std::invalid_argument& e) {
std::cerr << "Invalid argument when parsing lane_detection_blur_level: " << e.what() << std::endl;
} catch (const std::out_of_range& e) {
std::cerr << "Out of range when parsing lane_detection_blur_level: " << e.what() << std::endl;
}
}
if (command == "lane_detection_canny_thresh_low") {
try {
current->lane_detection_canny_thresh_low = std::stoi(arg);
} catch (const std::invalid_argument& e) {
std::cerr << "Invalid argument when parsing lane_detection_canny_thresh_low: " << e.what() << std::endl;
} catch (const std::out_of_range& e) {
std::cerr << "Out of range when parsing lane_detection_canny_thresh_low: " << e.what() << std::endl;
}
}
if (command == "lane_detection_canny_thresh_high") {
try {
current->lane_detection_canny_thresh_high = std::stoi(arg);
} catch (const std::invalid_argument& e) {
std::cerr << "Invalid argument when parsing lane_detection_canny_thresh_high: " << e.what() << std::endl;
} catch (const std::out_of_range& e) {
std::cerr << "Out of range when parsing lane_detection_canny_thresh_high: " << e.what() << std::endl;
}
}
if (command == "lane_perspective_floor") {
try {
current->lane_perspective_floor = std::stoi(arg);
} catch (const std::invalid_argument& e) {
std::cerr << "Invalid argument when parsing lane_perspective_floor: " << e.what() << std::endl;
} catch (const std::out_of_range& e) {
std::cerr << "Out of range when parsing lane_perspective_floor: " << e.what() << std::endl;
}
}
if (command == "lane_perspective_roof") {
try {
current->lane_perspective_roof = std::stoi(arg);
} catch (const std::invalid_argument& e) {
std::cerr << "Invalid argument when parsing lane_perspective_roof: " << e.what() << std::endl;
} catch (const std::out_of_range& e) {
std::cerr << "Out of range when parsing lane_perspective_roof: " << e.what() << std::endl;
}
}
if (command == "lane_perspective_startfloor") {
try {
current->lane_perspective_startfloor = std::stoi(arg);
} catch (const std::invalid_argument& e) {
std::cerr << "Invalid argument when parsing lane_perspective_startfloor: " << e.what() << std::endl;
} catch (const std::out_of_range& e) {
std::cerr << "Out of range when parsing lane_perspective_startfloor: " << e.what() << std::endl;
}
}
if (command == "lane_perspective_stopfloor") {
try {
current->lane_perspective_stopfloor = std::stoi(arg);
} catch (const std::invalid_argument& e) {
std::cerr << "Invalid argument when parsing lane_perspective_stopfloor: " << e.what() << std::endl;
} catch (const std::out_of_range& e) {
std::cerr << "Out of range when parsing lane_perspective_stopfloor: " << e.what() << std::endl;
}
}
if (command == "lane_perspective_startroof") {
try {
current->lane_perspective_startroof = std::stoi(arg);
} catch (const std::invalid_argument& e) {
std::cerr << "Invalid argument when parsing lane_perspective_startroof: " << e.what() << std::endl;
} catch (const std::out_of_range& e) {
std::cerr << "Out of range when parsing lane_perspective_startroof: " << e.what() << std::endl;
}
}
if (command == "lane_perspective_stoproof") {
try {
current->lane_perspective_stoproof = std::stoi(arg);
} catch (const std::invalid_argument& e) {
std::cerr << "Invalid argument when parsing lane_perspective_stoproof: " << e.what() << std::endl;
} catch (const std::out_of_range& e) {
std::cerr << "Out of range when parsing lane_perspective_stoproof: " << e.what() << std::endl;
}
}
if (command == "web_feed_post_server") {
current->web_feed_post_server = std::move(arg);
}
if (command == "web_feed_post_raw_image_path") {
current->web_feed_post_raw_image_path = std::move(arg);
}
