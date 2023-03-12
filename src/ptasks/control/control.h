#ifndef CONTROLLER_CONTROL_HPP
#define CONTROLLER_CONTROL_HPP

#include <ptasks/ptask.h>

class Control : public Ptask {
private:

public:
    Control();
    void compute(Pdata*, const Options*) override;

    void lane_correction(Pdata *pdata, const Options *options);
};


#endif //CONTROLLER_CONTROL_HPP
