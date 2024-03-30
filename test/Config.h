#ifndef Confg_h
#define Confg_h

#include "ETLBeamTestReco/interface/NTupleReader.h"
#include "ETLBeamTestReco/interface/Geometry.h"
#include "ETLBeamTestReco/interface/PrepNTupleVars.h"
#include "ETLBeamTestReco/interface/ETROC.h"
#include "ETLBeamTestReco/interface/Timing.h"
#include "ETLBeamTestReco/interface/Utility.h"

#include <iostream>
#include <fstream>

class Config
{
private:
    void registerModules(NTupleReader& tr, const std::vector<std::string>&& modules) const
    {        
        for(const auto& module : modules)
        {
            if     (module=="PrepNTupleVars")          tr.emplaceModule<PrepNTupleVars>(std::vector<std::shared_ptr<TProfile2D>>());
            else if(module=="ETROC")                   tr.emplaceModule<ETROC>();
            else if(module=="Timing")                  tr.emplaceModule<Timing>();
        }
    }

    template<typename T> void registerGeometry(NTupleReader& tr, const T& g) const
    {
        tr.registerDerivedVar("indexToGeometryMap", g.indexToGeometryMap);
        tr.registerDerivedVar("geometry", g.geometry);
        tr.registerDerivedVar("acLGADChannelMap", g.acLGADChannelMap);
        tr.registerDerivedVar("numLGADchannels", g.numLGADchannels);
        tr.registerDerivedVar("amplitudeCorrectionFactor", g.amplitudeCorrectionFactor);
        tr.registerDerivedVar("stripWidth",g.stripWidth);
        tr.registerDerivedVar("sensorCenter",g.sensorCenter); 
        tr.registerDerivedVar("sensorCenterY",g.sensorCenterY); 
        tr.registerDerivedVar("pitch",g.pitch); 
        tr.registerDerivedVar("stripCenterXPosition", g.stripCenterXPosition);
        tr.registerDerivedVar("stripCenterYPosition", g.stripCenterYPosition);
        tr.registerDerivedVar("timeCalibrationCorrection", g.timeCalibrationCorrection);
        tr.registerDerivedVar("extraChannelIndex", g.extraChannelIndex);
        tr.registerDerivedVar("photekIndex", g.photekIndex);
        tr.registerDerivedVar("lowGoodStripIndex", g.lowGoodStripIndex);
        tr.registerDerivedVar("highGoodStripIndex", g.highGoodStripIndex);
        tr.registerDerivedVar("CFD_threshold", g.CFD_threshold);
        tr.registerDerivedVar("CFD_list", g.CFD_list);
        tr.registerDerivedVar("sensorEdges", g.sensorEdges);
        tr.registerDerivedVar("sensorEdgesTight", g.sensorEdgesTight);
        tr.registerDerivedVar("sensorEdgesExtra", g.sensorEdgesExtra);
        tr.registerDerivedVar("alpha", g.alpha);
        tr.registerDerivedVar("beta",  g.beta);
        tr.registerDerivedVar("gamma", g.gamma);
        tr.registerDerivedVar("z_dut", g.z_dut);
        tr.registerDerivedVar("xBinSize", g.xBinSize);
        tr.registerDerivedVar("yBinSize", g.yBinSize);
        tr.registerDerivedVar("xBinSize_delay_corr", g.xBinSize_delay_corr);
        tr.registerDerivedVar("yBinSize_delay_corr", g.yBinSize_delay_corr);
        tr.registerDerivedVar("xmin", g.xmin);
        tr.registerDerivedVar("xmax", g.xmax);
        tr.registerDerivedVar("ymin", g.ymin);
        tr.registerDerivedVar("ymax", g.ymax);
        tr.registerDerivedVar("positionRecoMaxPoint", g.positionRecoMaxPoint);
        tr.registerDerivedVar("positionRecoMaxPointRow", g.positionRecoMaxPointRow);
        tr.registerDerivedVar("positionRecoMaxPointCol", g.positionRecoMaxPointCol);
        tr.registerDerivedVar("photekSignalThreshold", g.photekSignalThreshold);
        tr.registerDerivedVar("photekSignalMax", g.photekSignalMax);
        tr.registerDerivedVar("noiseAmpThreshold", g.noiseAmpThreshold);
        tr.registerDerivedVar("signalAmpThreshold", g.signalAmpThreshold);
        tr.registerDerivedVar("isPadSensor", g.isPadSensor);
        tr.registerDerivedVar("isHPKStrips", g.isHPKStrips);
        tr.registerDerivedVar("enablePositionReconstruction", g.enablePositionReconstruction);
        tr.registerDerivedVar("enablePositionReconstructionPad", g.enablePositionReconstructionPad);
        tr.registerDerivedVar("uses2022Pix", g.uses2022Pix);
        tr.registerDerivedVar("usesMay2023Tracker", g.usesMay2023Tracker);
        tr.registerDerivedVar("isHorizontal", g.isHorizontal);
        tr.registerDerivedVar("minPixHits", g.minPixHits);
        tr.registerDerivedVar("minStripHits", g.minStripHits);
        tr.registerDerivedVar("positionRecoPar", g.positionRecoPar);
        tr.registerDerivedVar("positionRecoParRow", g.positionRecoParRow);
        tr.registerDerivedVar("positionRecoParCol", g.positionRecoParCol);
        tr.registerDerivedVar("positionRecoParRight", g.positionRecoParRight);
        tr.registerDerivedVar("positionRecoParLeft", g.positionRecoParLeft);
        tr.registerDerivedVar("positionRecoParTop", g.positionRecoParTop);
        tr.registerDerivedVar("positionRecoParBot", g.positionRecoParBot);
        tr.registerDerivedVar("xSlices", g.xSlices);
        tr.registerDerivedVar("ySlices", g.ySlices);
        tr.registerDerivedVar("boxes_XY", g.boxes_XY);
        tr.registerDerivedVar("regionsOfIntrest", g.regionsOfIntrest);
    }

    int getVoltage(std::string name) const
    {
        std::vector<std::string> stringChunks;
        bool keepGoing = true;
        while(keepGoing)
        {
            const auto& first = utility::split("first", name, "_");
            const auto& last = utility::split("last", name, "_");
            stringChunks.emplace_back(first);
            if(last == "" || last == name)
            {
                keepGoing = false;
            }
            else 
            {
                name = last;
            }
        }

        std::string sVoltage;
        for(auto c : stringChunks)
        {
            sVoltage = utility::split("first", c, "V");
            if(sVoltage != c) break;
        }

        return std::stoi(sVoltage);;
    }

public:
    Config() 
    {
    }

    void setUp(NTupleReader& tr) const
    {
        //Get and make needed info
        const auto& filetag = tr.getVar<std::string>("filetag");
        const auto& analyzer = tr.getVar<std::string>("analyzer");

        std::string runYear = "2024";
        tr.registerDerivedVar("runYear",runYear);
        
        //Define zScan values and save to a python file for later
        const auto voltage = getVoltage(filetag);
        tr.registerDerivedVar("voltage", voltage);
        std::cout<<"Voltage: "<<voltage<<std::endl;

        //Setup Sensor Geometry 
        if(filetag.find("ETL_Test_100V_1") != std::string::npos) registerGeometry(tr, ETL_Test_100V_1_Geometry(voltage));
	    else
        {
            registerGeometry(tr, DefaultGeometry(voltage));
            std::cout<<"Warning: Using DefaultGeometry, odds are this is not what you want"<<std::endl;
        }

        //Register Modules that are needed for each Analyzer
        if (analyzer=="Analyze")
        {
            const std::vector<std::string> modulesList = {
                "PrepNTupleVars",
                "ETROC",
                "Timing",
            };
            registerModules(tr, std::move(modulesList));
        }
        else
        {
            const std::vector<std::string> modulesList = {
                "PrepNTupleVars",
                "ETROC",
                "Timing",
            };
            registerModules(tr, std::move(modulesList));
        }
    }
};

#endif
