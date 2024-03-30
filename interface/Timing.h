#ifndef TIMING_H
#define TIMING_H

#include "ETLBeamTestReco/interface/Utility.h"

class Timing
{
private:
    template<typename T> inline T getValue(bool cut, T value) const
    {
        return (cut) ? value : 0.0;
    }

    void timing([[maybe_unused]] NTupleReader& tr)
    {
        // const auto& ampLGAD = tr.getVec<std::vector<double>>("ampLGAD");
        // const auto& timeLGAD = tr.getVec<std::vector<double>>("timeLGAD");
        // const auto& signalAmpThreshold = tr.getVar<double>("signalAmpThreshold");
        // const auto& noiseAmpThreshold = tr.getVar<double>("noiseAmpThreshold");

        

    }
public:
    Timing()
    {
        std::cout<<"Running Timing Module"<<std::endl;
    }

    void operator()(NTupleReader& tr)
    {
        timing(tr);
    }
};

#endif
