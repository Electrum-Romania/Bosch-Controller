#ifndef PTASKS_ANALYSIS_H
#define PTASKS_ANALYSIS_H

#include "../ptask.h"
#include <opencv2/opencv.hpp>

class Analysis : public Ptask {
public:
    Analysis();
    void create_analysis(Pdata*, const Options*, cv::Mat&, const std::string&);
    void compute(Pdata*, const Options*) override;
};

#endif