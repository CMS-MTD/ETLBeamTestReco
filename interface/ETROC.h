#ifndef ETROC_H
#define ETROC_H

#include "TestbeamReco/interface/Utility.h"

class ETROC
{
private:
    void etroc([[maybe_unused]] NTupleReader& tr)
    {
        // Making photek variables
        const auto& photekIndex = tr.getVar<int>("photekIndex");
        const auto& corrTime = tr.getVec<double>("corrTime");
        const auto& corrAmp = tr.getVec<double>("corrAmp");
        auto& photekTime = tr.createDerivedVar<double>("photekTime");
        auto& photekAmp = tr.createDerivedVar<double>("photekAmp");
        photekTime = corrTime[photekIndex];
        photekAmp = corrAmp[photekIndex];
        
        // Making ETROC based variables
        const auto& cal_code = tr.getVec<float>("cal_code");
        const auto& toa_code = tr.getVec<float>("toa_code");
        const auto& tot_code = tr.getVec<float>("tot_code");
        const auto& Clock = tr.getVar<double>("Clock");

        auto& toaVec = tr.createDerivedVec<double>("toaVec");
        auto& totVec = tr.createDerivedVec<double>("totVec");
        auto& dtVec  = tr.createDerivedVec<double>("dtVec");

        for(unsigned int i = 0; i < cal_code.size(); i++)
        {
            double bin = 3.125 / cal_code[i];
            double toa = 12.5 - bin * toa_code[i];
            double tot = (2 * tot_code[i] - double(floor(tot_code[i]/32.0))) * bin;
            double dt  = toa - (photekTime - Clock);

            toaVec.emplace_back(toa);
            totVec.emplace_back(tot);
            dtVec.emplace_back(dt);
        }
    }

public:
    ETROC()
    {
        std::cout<<"Running ETROC Module"<<std::endl;
    }

    void operator()(NTupleReader& tr)
    {
        etroc(tr);
    }
};

#endif
