#ifndef PTASKS_ANALYSIS_H
#define PTASKS_ANALYSIS_H

#include <opencv2/opencv.hpp>

#include <ptasks/ptask.h>

class Analysis : public Ptask {
public:
    Analysis();
    static void create_analysis(Pdata*, const Options*, cv::Mat&, const std::string&);
    void compute(Pdata*, const Options*) override;
};

#endif
