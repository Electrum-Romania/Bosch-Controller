#ifndef PATSKS_LANE_DETECTION_H
#define PTASKS_LANE_DETECTION_H

#include "../ptask.h"

class LaneDetection : public Ptask {
public:
	void compute(Pdata*, const Options*) override;
};

#endif
