#include <ptasks/analysis/analysis.h>

Analysis::Analysis() : Ptask("Analysis", 'A') {

}

void Analysis::compute(Pdata* pdata, const Options* options)
{
    create_analysis(pdata, options, pdata->analysis1, options->analysis1_mode);
    create_analysis(pdata, options, pdata->analysis2, options->analysis2_mode);
}

cv::Mat draw_quad_mask(Pdata* pdata)
{
    int lineType = cv::LINE_8;
    cv::Mat img = pdata->camera_image;
    const cv::Point* ppt[1] = { &pdata->lanes_perspective_mask[0] };
    int npt[] = { 4 };

    // fillPoly( img,
    //     ppt,
    //     npt,
    //     1,
    //     cv::Scalar( 255, 255, 255 ),
    //     lineType );

    for(int i = 0; i < 4; i++) {
        if(i < 3) {
            cv::line(img, pdata->lanes_perspective_mask[i], pdata->lanes_perspective_mask[i+1], cv::Scalar(255, 255, 255), 5);
        }
        else if(i == 3) {
            cv::line(img, pdata->lanes_perspective_mask[i], pdata->lanes_perspective_mask[0], cv::Scalar(255, 255, 255), 5);
        }
    }

    return img;
}

cv::Mat create_blank_image() {
    cv::Mat blank(cv::Size(1920, 1080),CV_8UC1,cv::Scalar());
    return blank;
}

void Analysis::create_analysis(Pdata* pdata, const Options* options, cv::Mat& output, const std::string& mode)
{
    // std::cout<<mode<<"a"<<std::endl;
    if (mode == "raw") {
        output = pdata->camera_image;
    }
    else if(mode == "lanes_raw") {
        output = pdata->camera_image;
    }
    else if(mode == "lanes_perspective") {
        output = pdata->lanes_perspective;
    }
    else if(mode == "lanes_perspective-mask") {
        output = draw_quad_mask(pdata);
    }
    else if(mode == "lanes_alo") {
        output = create_blank_image();
    }
    else if(mode == "lanes_gray") {
        output = pdata->lanes_gray;
    }
    else if(mode == "lanes_white") {
        output = pdata->lanes_white;
    }
    else if(mode == "lanes_result") {
        output = pdata->lanes_result;
    }
    else if(mode == "lanes_window") {
        output = pdata->lanes_window;
    }
    else {
        output = pdata->camera_image;
    }
}

// cv::Mat draw_triangle_mask(cv::Size size, std::vector<cv::Point> points)
// {
// 	return;
// }
