#define Analyze_cxx
#include "TestbeamReco/interface/Analyze.h"
#include "TestbeamReco/interface/Utility.h"
#include "TestbeamReco/interface/NTupleReader.h"

#include <TH1D.h>
#include <TH2D.h>
#include <TEfficiency.h>
#include <TFile.h>
#include <iostream>
#include <fstream>

Analyze::Analyze()
{
}

//Define all your histograms here. 
void Analyze::InitHistos(NTupleReader& tr, const std::vector<std::vector<int>>& geometry)
{
    TH1::SetDefaultSumw2();
    TH2::SetDefaultSumw2();

    //This event counter histogram is necessary so that we know that all the condor jobs ran successfully. If not, when you use the hadder script, you will see a discrepancy in red as the files are being hadded.
    my_histos.emplace( "EventCounter", std::make_shared<TH1D>( "EventCounter", "EventCounter", 2, -1.1, 1.1 ) ) ;

    //Define 1D histograms
    const auto& xBinSize = tr.getVar<double>("xBinSize");
    const auto& yBinSize = tr.getVar<double>("yBinSize");
    const auto& xmin = tr.getVar<double>("xmin");
    const auto& xmax = tr.getVar<double>("xmax");
    const auto& ymin = tr.getVar<double>("ymin");
    const auto& ymax = tr.getVar<double>("ymax");
    const auto& acLGADChannelMap = tr.getVar<std::map<int, bool>>("acLGADChannelMap");
    int xbins = 175;
    int ybins = 175;

    int timeDiffNbin = 200; // 200
    double timeDiffLow = -1.0;
    double timeDiffHigh = 1.0;
    int timeDiffYnbin = 50;

    int    bvNbin = 500;
    double bvLow  = 0.0;
    double bvHigh = 500.0;

    int rowIndex = 0;
    for(const auto& row : geometry)
    {
        if(row.size()<2) continue;
        for(unsigned int i = 0; i < row.size(); i++)
        {
            if(!acLGADChannelMap.at(row[i])) continue;
            const auto& r = std::to_string(rowIndex);
            const auto& s = std::to_string(i);

            //Define 1D histograms
            utility::makeHisto(my_histos,"amp"+r+s,"", 450,-50.0,400.0);
            utility::makeHisto(my_histos,"time"+r+s, "", 500, -225.0, -175.0);
            utility::makeHisto(my_histos,"timeDiff_channel"+r+s, "", timeDiffNbin,timeDiffLow,timeDiffHigh);
            utility::makeHisto(my_histos,"timeDiffTracker_channel"+r+s, "", timeDiffNbin,timeDiffLow,timeDiffHigh);
            utility::makeHisto(my_histos,"baselineRMS"+r+s,"", 200,-10.0,10.0);
            utility::makeHisto(my_histos,"risetime"+r+s,"", 150,0.0,1500.0);
            utility::makeHisto(my_histos,"charge"+r+s,"", 300,0.0,150.0);
            utility::makeHisto(my_histos,"ampChargeRatio"+r+s,"", 300,0.0,15.0);
            utility::makeHisto(my_histos,"slewrate"+r+s,"", 300,0.0,400.0);
            utility::makeHisto(my_histos,"slewRateChargeRatio"+r+s,"", 300,0.0,30.0);
           
            //Define 2D histograms
            utility::makeHisto(my_2d_histos,"delay_vs_x_channel_top"+r+s, "; X [mm]; Arrival time [ns]", std::round((xmax-xmin)/xBinSize),xmin,xmax, 100,-11,-10);
            utility::makeHisto(my_2d_histos,"timeDiff_vs_x_channel"+r+s, "", std::round((xmax-xmin)/xBinSize),xmin,xmax, timeDiffNbin,timeDiffLow,timeDiffHigh);
            utility::makeHisto(my_2d_histos,"timeDiffTracker_vs_x_channel"+r+s, "", std::round((xmax-xmin)/xBinSize),xmin,xmax, timeDiffNbin,timeDiffLow,timeDiffHigh);
            utility::makeHisto(my_2d_histos,"risetime_vs_amp"+r+s, "; amp [mV]; risetime [ps]", 100,0.0,150.0, 100,100.0,1500.0);
            utility::makeHisto(my_2d_histos,"baselineRMS_vs_x_channel"+r+s, "; X [mm]; amp", std::round((xmax-xmin)/xBinSize),xmin,xmax, 250,0.0,500);
            utility::makeHisto(my_2d_histos,"amp_vs_x_channel"+r+s, "; X [mm]; amp", std::round((xmax-xmin)/xBinSize),xmin,xmax, 250,0.0,500);
            utility::makeHisto(my_2d_histos,"amp_vs_y_channel"+r+s, "; Y [mm]; amp", std::round((ymax-ymin)/yBinSize),ymin,ymax, 250,0.0,500);
            utility::makeHisto(my_2d_histos,"stripBoxInfo"+r+s, "", 1,-9999.0,9999.0, 1,-9999.9,9999.9);
            utility::makeHisto(my_2d_histos,"stripBoxInfoY"+r+s, "", 1,-9999.0,9999.0, 1,-9999.9,9999.9);

            utility::makeHisto(my_2d_histos,"timeDiff_vs_BV_channel"+r+s,    "; BV [V]; #Delta t [ps]", bvNbin,bvLow,bvHigh, timeDiffNbin,timeDiffLow,timeDiffHigh);
            utility::makeHisto(my_2d_histos,"amp_vs_BV_channel"+r+s,         "; BV [V]; amp [mV]"     , bvNbin,bvLow,bvHigh, 250,0.0,500);
            utility::makeHisto(my_2d_histos,"risetime_vs_BV_channel"+r+s,    "; BV [V]; risetime [ps]", bvNbin,bvLow,bvHigh, 100,100.0,1500.0);
            utility::makeHisto(my_2d_histos,"baselineRMS_vs_BV_channel"+r+s, "; BV [V]; amp [mV]"     , bvNbin,bvLow,bvHigh, 250,0.0,250);
            utility::makeHisto(my_2d_histos,"slewrate_vs_BV_channel"+r+s,    "; BV [V]; slewrate"     , bvNbin,bvLow,bvHigh, 300,0.0,400.0);
            utility::makeHisto(my_2d_histos,"jitter_vs_BV_channel"+r+s,      "; BV [V]; jitter [ps]"  , bvNbin,bvLow,bvHigh, 200,0.0,100.0);
            utility::makeHisto(my_2d_histos,"efficiency_vs_xy_numerator_channel"+r+s, "; X [mm]; Y [mm]", 2*std::round((xmax-xmin)/xBinSize),xmin,xmax, std::round((ymax-ymin)/yBinSize),ymin,ymax);
            utility::makeHisto(my_2d_histos,"efficiency_vs_xy_numerator_tight_channel"+r+s, "; X [mm]; Y [mm]", 2*std::round((xmax-xmin)/xBinSize),xmin,xmax, std::round((ymax-ymin)/yBinSize),ymin,ymax);

            //Define 3D histograms
            utility::makeHisto(my_3d_histos,"baselineRMS_vs_xy_channel"+r+s,"; X [mm]; Y [mm]",std::round((xmax-xmin)/xBinSize),xmin,xmax, std::round((ymax-ymin)/yBinSize),ymin,ymax, 250,0,500);
            utility::makeHisto(my_3d_histos,"amplitude_vs_xy_channel"+r+s,"; X [mm]; Y [mm]",std::round((xmax-xmin)/xBinSize),xmin,xmax, std::round((ymax-ymin)/yBinSize),ymin,ymax, 250,0,500);
            utility::makeHisto(my_3d_histos,"raw_amp_vs_xy_channel"+r+s,"; X [mm]; Y [mm]",std::round((xmax-xmin)/xBinSize),xmin,xmax, std::round((ymax-ymin)/yBinSize),ymin,ymax, 250,0,500);
            utility::makeHisto(my_3d_histos,"timeDiff_vs_xy_channel"+r+s, "; X [mm]; Y [mm]",std::round((xmax-xmin)/xBinSize),xmin,xmax, std::round((ymax-ymin)/yBinSize),ymin,ymax, timeDiffNbin,timeDiffLow,timeDiffHigh);
            utility::makeHisto(my_3d_histos,"timeDiffTracker_vs_xy_channel"+r+s, "; X [mm]; Y [mm]",std::round((xmax-xmin)/xBinSize),xmin,xmax, std::round((ymax-ymin)/yBinSize),ymin,ymax, timeDiffNbin,timeDiffLow,timeDiffHigh);
            utility::makeHisto(my_3d_histos,"risetime_vs_xy_channel"+r+s,"; X [mm]; Y [mm]",std::round((xmax-xmin)/xBinSize),xmin,xmax, std::round((ymax-ymin)/yBinSize),ymin,ymax, 300,0.0,1500.0);
            utility::makeHisto(my_3d_histos,"charge_vs_xy_channel"+r+s,"; X [mm]; Y [mm]",std::round((xmax-xmin)/xBinSize),xmin,xmax, std::round((ymax-ymin)/yBinSize),ymin,ymax,300,0.0,150.0);
            utility::makeHisto(my_3d_histos,"ampChargeRatio_vs_xy_channel"+r+s,"; X [mm]; Y [mm]",std::round((xmax-xmin)/xBinSize),xmin,xmax, std::round((ymax-ymin)/yBinSize),ymin,ymax,300,0.0,100.0);
            utility::makeHisto(my_3d_histos,"slewRate_vs_xy_channel"+r+s,"; X [mm]; Y [mm]",std::round((xmax-xmin)/xBinSize),xmin,xmax, std::round((ymax-ymin)/yBinSize),ymin,ymax,300,0.0,500.0);
        }
        rowIndex++;
    }
    //Global 1D histograms
    utility::makeHisto(my_histos,"nhits", "; nhits; Events", 20, 0, 20);
    utility::makeHisto(my_histos,"cal", "; CAL_code; Events", 300, 0.0, 300.0);
    utility::makeHisto(my_histos,"toa", "; TOA [ns]; Events", 500, 0.0, 50.0);
    utility::makeHisto(my_histos,"tot", "; TOT [ns]; Events", 500, 0.0, 50.0);
    utility::makeHisto(my_histos,"dt", "; dt [ns]; Events", 500, 9.0, 13.0);

    utility::makeHisto(my_histos,"ntracks_alt", "; ntracks_alt; Events", 10,0,10);
    utility::makeHisto(my_histos,"chi2", "; Track chi2/ndf; Events", 60,0,30);
    utility::makeHisto(my_histos,"nplanes", "; nplanes; Events", 20,0,20);
    utility::makeHisto(my_histos,"npix", "; npix; Events", 4,0,4);
    utility::makeHisto(my_histos,"monicelli_err", "; Track expected error; Events", 100,0,15);
    utility::makeHisto(my_histos,"slopeX", "; Track X slope [mm per mm]; Events", 100,-0.001,0.001);
    utility::makeHisto(my_histos,"slopeY", "; Track Y slope [mm per mm]; Events", 100,-0.001,0.001);
    utility::makeHisto(my_histos,"ampRank1", "", 450, -50.0, 400.0);
    utility::makeHisto(my_histos,"ampRank2", "", 450, -50.0, 400.0);
    utility::makeHisto(my_histos,"ampRank3", "", 450, -50.0, 400.0);
    utility::makeHisto(my_histos,"ampRank4", "", 450, -50.0, 400.0);
    utility::makeHisto(my_histos,"ampRank5", "", 450, -50.0, 400.0);
    utility::makeHisto(my_histos,"ampRank6", "", 450, -50.0, 400.0);
    utility::makeHisto(my_histos,"wave0", "; Time [ns]; Voltage [mV]", 500,-25.0,25.0);
    utility::makeHisto(my_histos,"wave1", "; Time [ns]; Voltage [mV]", 500,-25.0,25.0);
    utility::makeHisto(my_histos,"wave2", "; Time [ns]; Voltage [mV]", 500,-25.0,25.0);
    utility::makeHisto(my_histos,"wave3", "; Time [ns]; Voltage [mV]", 500,-25.0,25.0);
    utility::makeHisto(my_histos,"wave4", "; Time [ns]; Voltage [mV]", 500,-25.0,25.0);
    utility::makeHisto(my_histos,"wave5", "; Time [ns]; Voltage [mV]", 500,-25.0,25.0);
    utility::makeHisto(my_histos,"wave6", "; Time [ns]; Voltage [mV]", 500,-25.0,25.0);
    utility::makeHisto(my_histos,"wave7", "; Time [ns]; Voltage [mV]", 500,-25.0,25.0);

    // --- Timing ---
    utility::makeHisto(my_histos,"timePhotek", "", 400, -20.0, 20.0);
    utility::makeHisto(my_histos,"Clock", "", 400, -20.0, 20.0);
    utility::makeHisto(my_histos,"timeDiff", "", timeDiffNbin,timeDiffLow,timeDiffHigh);

    // --- General quantities ---
    utility::makeHisto(my_histos,"risetime","", 150,0.0,1500.0);
    utility::makeHisto(my_histos,"charge","", 300,0.0,150.0);
    utility::makeHisto(my_histos,"ampChargeRatio","", 300,0.0,15.0);
    utility::makeHisto(my_histos,"index_diff", "; index1 - index2; Events", 16,-8,8);
    utility::makeHisto(my_histos,"x", "; x_{track} [mm]; Events", 100,-5.5,5.5);
    utility::makeHisto(my_histos,"y", "; Y_{track} [mm]; Events", 100,-5.5,5.5);

    //Global 2D histograms
    utility::makeHisto(my_2d_histos,"Amp1_vs_x", "; X [mm]; Amp1", std::round((xmax-xmin)/xBinSize),xmin,xmax, 250,0.0,500);
    utility::makeHisto(my_2d_histos,"Amp2_vs_x", "; X [mm]; Amp2", std::round((xmax-xmin)/xBinSize),xmin,xmax, 250,0.0,500);
    utility::makeHisto(my_2d_histos,"BaselineRMS12_vs_x", "; X [mm]; Noise Sum 12", std::round((xmax-xmin)/xBinSize),xmin,xmax, 40,0.0,10);
    utility::makeHisto(my_2d_histos,"row_vs_col", "; row; col", 16,0,16, 16,0,16);

    //Global 3D histograms
    utility::makeHisto(my_3d_histos,"dt_vs_row_col", "; X [mm]; Y [MM]", 16,0,16, 16,0,16, 500,9.0,13.0);
    utility::makeHisto(my_3d_histos,"nhits_vs_xy", "; X [mm]; Y [MM]", std::round((xmax-xmin)/xBinSize),xmin,xmax, std::round((ymax-ymin)/yBinSize),ymin,ymax, 20, 0, 20);
    utility::makeHisto(my_3d_histos,"amplitude_vs_xy","; X [mm]; Y [mm]",std::round((xmax-xmin)/xBinSize),xmin,xmax, std::round((ymax-ymin)/yBinSize),ymin,ymax, 250,0,500);
    utility::makeHisto(my_3d_histos,"amplitude_vs_xy_tight","; X [mm]; Y [mm]",std::round((xmax-xmin)/xBinSize),xmin,xmax, std::round((ymax-ymin)/yBinSize),ymin,ymax, 250,0,500);
    utility::makeHisto(my_3d_histos,"timeDiff_vs_xy", "; X [mm]; Y [mm]", std::round((xmax-xmin)/xBinSize),xmin,xmax, timeDiffYnbin,ymin,ymax, timeDiffNbin,timeDiffLow,timeDiffHigh);
    utility::makeHisto(my_3d_histos,"timeDiffTracker_vs_xy", "; X [mm]; Y [mm]", std::round((xmax-xmin)/xBinSize),xmin,xmax, timeDiffYnbin,ymin,ymax, timeDiffNbin,timeDiffLow,timeDiffHigh);

    utility::makeHisto(my_3d_histos,"risetime_vs_xy","; X [mm]; Y [mm]",std::round((xmax-xmin)/xBinSize),xmin,xmax, std::round((ymax-ymin)/yBinSize),ymin,ymax, 150,0.0,1500.0);
    utility::makeHisto(my_3d_histos,"risetime_vs_xy_tight","; X [mm]; Y [mm]",std::round((xmax-xmin)/xBinSize),xmin,xmax, std::round((ymax-ymin)/yBinSize),ymin,ymax, 150,0.0,1500.0);
    utility::makeHisto(my_3d_histos,"baselineRMS_vs_xy","; X [mm]; Y [mm]",std::round((xmax-xmin)/xBinSize),xmin,xmax, std::round((ymax-ymin)/yBinSize),ymin,ymax, 150,0.0,50.0);
    utility::makeHisto(my_3d_histos,"charge_vs_xy","; X [mm]; Y [mm]",std::round((xmax-xmin)/xBinSize),xmin,xmax, std::round((ymax-ymin)/yBinSize),ymin,ymax,300,0.0,150.0);
    utility::makeHisto(my_3d_histos,"ampChargeRatio_vs_xy","; X [mm]; Y [mm]",std::round((xmax-xmin)/xBinSize),xmin,xmax, std::round((ymax-ymin)/yBinSize),ymin,ymax,300,0.0,50.0);
    utility::makeHisto(my_3d_histos,"slewRate_vs_xy","; X [mm]; Y [mm]",std::round((xmax-xmin)/xBinSize),xmin,xmax, std::round((ymax-ymin)/yBinSize),ymin,ymax,300,0.0,500.0);

    utility::makeHisto(my_3d_histos,"timeDiff_vs_xy_tight", "; X [mm]; Y [mm]", std::round((xmax-xmin)/xBinSize),xmin,xmax, timeDiffYnbin,ymin,ymax, timeDiffNbin,timeDiffLow,timeDiffHigh);
   
    //Efficiency
    utility::makeHisto(my_2d_histos,"efficiency_vs_xy_denominator", "; X [mm]; Y [mm]", 2*std::round((xmax-xmin)/xBinSize),xmin,xmax, std::round((ymax-ymin)/yBinSize),ymin,ymax);
    utility::makeHisto(my_2d_histos,"efficiency_vs_xy_numerator", "; X [mm]; Y [mm]", 2*std::round((xmax-xmin)/xBinSize),xmin,xmax, std::round((ymax-ymin)/yBinSize),ymin,ymax);
    utility::makeHisto(my_2d_histos,"efficiency_vs_xy_denominator_coarseBins", "; X [mm]; Y [mm]", std::round((xmax-xmin)/xBinSize),xmin,xmax, std::round((ymax-ymin)/yBinSize),ymin,ymax);
    utility::makeHisto(my_2d_histos,"efficiency_vs_xy_numerator_coarseBins", "; X [mm]; Y [mm]", std::round((xmax-xmin)/xBinSize),xmin,xmax, std::round((ymax-ymin)/yBinSize),ymin,ymax);
    utility::makeHisto(my_2d_histos,"efficiency_vs_xy_denominator_tight", "; X [mm]; Y [mm]", 2*std::round((xmax-xmin)/xBinSize),xmin,xmax, std::round((ymax-ymin)/yBinSize),ymin,ymax);
    utility::makeHisto(my_2d_histos,"efficiency_vs_xy_numerator_tight", "; X [mm]; Y [mm]", 2*std::round((xmax-xmin)/xBinSize),xmin,xmax, std::round((ymax-ymin)/yBinSize),ymin,ymax);
    utility::makeHisto(my_2d_histos,"efficiency_vs_xy_denominator_coarseBins_tight", "; X [mm]; Y [mm]", std::round((xmax-xmin)/xBinSize),xmin,xmax, std::round((ymax-ymin)/yBinSize),ymin,ymax);
    utility::makeHisto(my_2d_histos,"efficiency_vs_xy_numerator_coarseBins_tight", "; X [mm]; Y [mm]", std::round((xmax-xmin)/xBinSize),xmin,xmax, std::round((ymax-ymin)/yBinSize),ymin,ymax);

    utility::makeHisto(my_1d_prof,"waveProf0", "; Time [ns]; Voltage [mV]", 500,0.0,25.0);
    utility::makeHisto(my_1d_prof,"waveProf1", "; Time [ns]; Voltage [mV]", 500,0.0,25.0);
    utility::makeHisto(my_1d_prof,"waveProf2", "; Time [ns]; Voltage [mV]", 500,0.0,25.0);
    utility::makeHisto(my_1d_prof,"waveProf3", "; Time [ns]; Voltage [mV]", 500,0.0,25.0);
    utility::makeHisto(my_1d_prof,"waveProf4", "; Time [ns]; Voltage [mV]", 500,0.0,25.0);
    utility::makeHisto(my_1d_prof,"waveProf5", "; Time [ns]; Voltage [mV]", 500,0.0,25.0);
    utility::makeHisto(my_1d_prof,"waveProf6", "; Time [ns]; Voltage [mV]", 500,0.0,25.0);
    utility::makeHisto(my_1d_prof,"waveProf7", "; Time [ns]; Voltage [mV]", 500,0.0,25.0);
       
    utility::makeHisto(my_2d_prof,"efficiency_vs_xy_ETROC", "; X [mm]; Y [MM]", std::round((xmax-xmin)/xBinSize),xmin,xmax, std::round((ymax-ymin)/yBinSize),ymin,ymax);

    //Define TEfficiencies if you are doing trigger studies (for proper error bars) or cut flow charts.
    utility::makeHisto(my_efficiencies,"efficiency_vs_x","; X [mm]",xbins,xmin,xmax);
    utility::makeHisto(my_efficiencies,"efficiency_vs_xy","; X [mm]; Y [mm]",xbins,xmin,xmax, ybins,ymin,ymax);
    std::cout<<"Finished defining histos"<<std::endl;
}

//Put everything you want to do per event here.
void Analyze::Loop(NTupleReader& tr, int maxevents)
{
    const auto& indexToGeometryMap = tr.getVar<std::map<int, std::vector<int>>>("indexToGeometryMap");
    const auto& geometry = tr.getVar<std::vector<std::vector<int>>>("geometry");
    const auto& photekSignalThreshold = tr.getVar<double>("photekSignalThreshold");
    const auto& noiseAmpThreshold = tr.getVar<double>("noiseAmpThreshold");
    const auto& signalAmpThreshold = tr.getVar<double>("signalAmpThreshold");
    const auto& photekSignalMax = tr.getVar<double>("photekSignalMax");
    const auto& sensorEdges = tr.getVar<std::vector<std::vector<double>>>("sensorEdges");
    const auto& firstFile = tr.getVar<bool>("firstFile");
    const auto& voltage = tr.getVar<int>("voltage");
    bool plotWaveForm = false;

    if(firstFile) InitHistos(tr, geometry);

    while( tr.getNextEvent() )
    {
        //This is added to count the number of events- do not change the next two lines.
        const auto& eventCounter = tr.getVar<int>("eventCounter");
        my_histos["EventCounter"]->Fill( eventCounter );

        //Print Event Number 
        if( maxevents != -1 && tr.getEvtNum() >= maxevents ) break;
        if( tr.getEvtNum() % 100000 == 0 ) printf( " Event %i\n", tr.getEvtNum() );
                       
        //Can add some fun code here....try not to calculate too much in this file: use modules to do the heavy calculations
        /*const auto& channel = tr.getVecVec<float>("channel");
        const auto& time_real = tr.getVecVec<float>("time");*/
        const auto& ampLGAD = tr.getVec<std::vector<double>>("ampLGAD");
        const auto& rawAmpLGAD = tr.getVec<std::vector<float>>("rawAmpLGAD");
        const auto& timeLGAD = tr.getVec<std::vector<double>>("timeLGAD");
        const auto& timeLGADTracker = tr.getVec<std::vector<double>>("timeLGADTracker");

        //const auto& CFD_list = tr.getVar<std::vector<std::string> >("CFD_list");
        const auto& baselineRMS = tr.getVec<std::vector<float>>("baselineRMS");
        const auto& risetimeLGAD = tr.getVec<std::vector<double>>("risetimeLGAD");
        const auto& chargeLGAD = tr.getVec<std::vector<double>>("chargeLGAD");
        const auto& ampChargeRatioLGAD = tr.getVec<std::vector<double>>("ampChargeRatioLGAD");
        const auto& jitterLGAD = tr.getVec<std::vector<double>>("jitterLGAD");
        const auto& slewrateLGAD = tr.getVec<std::vector<double>>("slewrateLGAD");
        const auto& slewRateChargeRatioLGAD = tr.getVec<std::vector<double>>("slewRateChargeRatioLGAD");
        const auto& ntracks = tr.getVar<int>("ntracks");
        const auto& ntracks_alt = tr.getVar<int>("ntracks_alt");
        const auto& nplanes = tr.getVar<int>("nplanes");
        const auto& npix = tr.getVar<int>("npix");
        const auto& chi2 = tr.getVar<float>("chi2");
        const auto& x = tr.getVar<float>("x");
        const auto& y = tr.getVar<float>("y");
        const auto& hitSensor = tr.getVar<bool>("hitSensor");
        const auto& Clock = tr.getVar<double>("Clock");
        const auto& nhits = tr.getVar<int>("nhits");
        const auto& col = tr.getVec<int>("col");
        const auto& row = tr.getVec<int>("row");
        const auto& cal_code = tr.getVec<float>("cal_code");
        const auto& toaVec = tr.getVec<double>("toaVec");
        const auto& totVec = tr.getVec<double>("totVec");
        const auto& dtVec  = tr.getVec<double>("dtVec");
        const auto& photekTime = tr.getVar<double>("photekTime");
        const auto& photekAmp = tr.getVar<double>("photekAmp");

        // Define selection bools
        bool goodPhotek = photekAmp > photekSignalThreshold && photekAmp < photekSignalMax;
        bool goodTrack = true; //ntracks==1 && nplanes>=14 && npix>0 && chi2 < 3.0;// && ntracks_alt==1;
        bool pass = goodTrack && hitSensor && goodPhotek;

        double firstEvent = tr.isFirstEvent();

        //******************************************************************
        // Make cuts and fill histograms here
        //******************************************************************
        // Loop over each channel in each sensor
        int rowIndex = 0;
        for(const auto& row : ampLGAD)
        {
            for(unsigned int i = 0; i < row.size(); i++)
            {
                const auto& r = std::to_string(rowIndex);
                const auto& s = std::to_string(i);
                auto passChannel = pass;
                const auto& ampChannel = ampLGAD[rowIndex][i];
                const auto& rawAmpChannel = rawAmpLGAD[rowIndex][i];
                const auto& noise = baselineRMS[rowIndex][i];
                const auto& risetime = risetimeLGAD[rowIndex][i];
                const auto& charge = chargeLGAD[rowIndex][i];
                const auto& ampChargeRatio = ampChargeRatioLGAD[rowIndex][i];
                const auto& jitter = jitterLGAD[rowIndex][i];
                const auto& slewrate = slewrateLGAD[rowIndex][i];
                const auto& slewRateChargeRatio = slewRateChargeRatioLGAD[rowIndex][i];
                bool goodNoiseAmp = ampChannel>noiseAmpThreshold;
                double time = timeLGAD[rowIndex][i];
                double timeTracker = timeLGADTracker[rowIndex][i];
                bool goodHit = goodNoiseAmp;
                                 
                utility::fillHisto(passChannel,                         my_histos, "amp"+r+s, ampChannel);
                utility::fillHisto(passChannel,                         my_histos, "time"+r+s, time);
                utility::fillHisto(passChannel,                         my_histos, "timeDiff_channel"+r+s, time-photekTime);
                utility::fillHisto(passChannel,                         my_histos, "timeDiffTracker_channel"+r+s, timeTracker-photekTime);
                utility::fillHisto(passChannel,                         my_histos, "baselineRMS"+r+s, noise);
                utility::fillHisto(passChannel,                         my_histos, "risetime"+r+s, risetime);
                utility::fillHisto(passChannel,                         my_histos, "charge"+r+s, charge);
                utility::fillHisto(passChannel,                         my_histos, "ampChargeRatio"+r+s, ampChargeRatio);
                utility::fillHisto(passChannel,                         my_histos, "slewRateChargeRatio"+r+s, slewRateChargeRatio);
                utility::fillHisto(passChannel,                         my_histos, "slewrate"+r+s, slewrate);
                utility::fillHisto(passChannel,                         my_2d_histos, "baselineRMS_vs_x_channel"+r+s, x,noise);
                utility::fillHisto(passChannel,                         my_2d_histos, "amp_vs_x_channel"+r+s, x,ampChannel);
                utility::fillHisto(passChannel,                         my_2d_histos, "risetime_vs_amp"+r+s,ampChannel, risetime);
                utility::fillHisto(passChannel,                         my_2d_histos, "amp_vs_y_channel"+r+s, y,ampChannel);
                utility::fillHisto(passChannel,                         my_2d_histos, "timeDiff_vs_BV_channel"+r+s, voltage,time-photekTime);
                utility::fillHisto(passChannel,                         my_2d_histos, "amp_vs_BV_channel"+r+s, voltage,ampChannel);
                utility::fillHisto(passChannel,                         my_2d_histos, "risetime_vs_BV_channel"+r+s, voltage,risetime);
                utility::fillHisto(passChannel,                         my_2d_histos, "baselineRMS_vs_BV_channel"+r+s, voltage,noise);
                utility::fillHisto(passChannel,                         my_2d_histos, "slewrate_vs_BV_channel"+r+s, voltage,slewrate);
                utility::fillHisto(passChannel,                         my_2d_histos, "jitter_vs_BV_channel"+r+s, voltage,jitter);
                utility::fillHisto(passChannel,                         my_2d_histos, "timeDiff_vs_x_channel"+r+s, x,time-photekTime);
                utility::fillHisto(passChannel,                         my_2d_histos, "timeDiffTracker_vs_x_channel"+r+s, x,timeTracker-photekTime);
                utility::fillHisto(passChannel,                         my_3d_histos, "baselineRMS_vs_xy_channel"+r+s, x,y,noise);
                utility::fillHisto(passChannel,                         my_3d_histos, "amplitude_vs_xy_channel"+r+s, x,y,ampChannel);
                utility::fillHisto(passChannel,                         my_3d_histos, "raw_amp_vs_xy_channel"+r+s, x,y,rawAmpChannel);
                utility::fillHisto(passChannel,                         my_3d_histos, "timeDiff_vs_xy_channel"+r+s, x,y,time-photekTime);
                utility::fillHisto(passChannel,                         my_3d_histos, "timeDiffTracker_vs_xy_channel"+r+s, x,y,timeTracker-photekTime);
                utility::fillHisto(passChannel,                         my_3d_histos, "risetime_vs_xy_channel"+r+s, x,y,risetime);
                utility::fillHisto(passChannel,                         my_3d_histos, "charge_vs_xy_channel"+r+s, x,y,charge);
                utility::fillHisto(passChannel,                         my_3d_histos, "ampChargeRatio_vs_xy_channel"+r+s, x,y,ampChargeRatio);
                utility::fillHisto(passChannel,                         my_3d_histos, "slewRate_vs_xy_channel"+r+s, x,y,slewrate);
                utility::fillHisto(passChannel,                         my_2d_histos, "efficiency_vs_xy_numerator_channel"+r+s, x,y);
                utility::fillHisto(goodTrack,                           my_efficiencies, "efficiency_vs_x", goodHit,x);
                utility::fillHisto(goodTrack,                           my_efficiencies, "efficiency_vs_xy", goodHit,x,y);
            }
            rowIndex++;
        }

        // --- Tracker ---
        utility::fillHisto(pass,                   my_histos, "nhits", nhits);
        utility::fillHisto(pass,                   my_histos, "chi2", chi2);
        utility::fillHisto(pass,                   my_histos, "ntracks_alt", ntracks_alt);
        utility::fillHisto(pass,                   my_histos, "nplanes", nplanes);
        utility::fillHisto(pass,                   my_histos, "npix", npix);
        utility::fillHisto(pass,                   my_histos, "x", x);
        utility::fillHisto(pass,                   my_histos, "y", y);

        // --- Timing ---
        utility::fillHisto(pass,                   my_histos, "Clock",Clock);
        utility::fillHisto(pass,                   my_histos, "timePhotek",photekTime);
        utility::fillHisto(pass,                   my_histos, "timeDiff", photekTime);

        // Save 2d histos

        // Save 3d histos
        utility::fillHisto(pass,                   my_3d_histos, "timeDiff_vs_xy", x,y,photekTime);
        utility::fillHisto(pass,                   my_3d_histos, "timeDiffTracker_vs_xy", x,y,photekTime);

        // Save profiles and efficiency histos
        utility::fillHisto(pass,                   my_2d_histos, "efficiency_vs_xy_denominator", x,y);

        // Plot ETROC histograms
        for(unsigned int i = 0; i < nhits; i++)
        {
            auto goodETROCHit = totVec[i]>3 && nhits>0 && cal_code[i] > 150 && cal_code[i] < 180 && toaVec[i] > 7 && toaVec[i] < 11;
            utility::fillHisto(pass,                   my_2d_histos, "row_vs_col", row[i],col[i]);
            utility::fillHisto(pass,                   my_histos, "cal", cal_code[i]);
            utility::fillHisto(pass,                   my_histos, "toa", toaVec[i]);
            utility::fillHisto(pass,                   my_histos, "tot", totVec[i]);
            utility::fillHisto(pass,                   my_histos, "dt", dtVec[i]);

            utility::fillHisto(goodETROCHit,      my_3d_histos, "dt_vs_row_col", row[i],col[i],dtVec[i]);
        }
        utility::fillHisto(nhits <= 1,             my_2d_prof,   "efficiency_vs_xy_ETROC", x,y,nhits == 1);
        utility::fillHisto(pass,                   my_3d_histos, "nhits_vs_xy", x,y,nhits>0);
        
        // Fill wave form histos once
        if(plotWaveForm)
        {
            const auto& channel = tr.getVecVec<float>("channel");
            const auto& time = tr.getVecVec<float>("time");
            const auto& timeCalibrationCorrection = tr.getVar<std::map<int, double>>("timeCalibrationCorrection");             
            for(unsigned int i = 0; i < channel.size(); i++)
            {
                auto t = timeCalibrationCorrection.at(i) - 10.0;
                if(i==7) continue;
                std::string index = std::to_string(i);
                for(unsigned int j = 0; j < time[0].size(); j++)
                {
                    my_1d_prof["waveProf"+index]->Fill(1e9*time[0][j] - photekTime - t, channel[i][j]);
                }
            }
        }	// Example Fill event selection efficiencies
    } //event loop
}

void Analyze::WriteHistos(TFile* outfile)
{
    outfile->cd();
    for(const auto& p : my_histos)       p.second->Write();
    for(const auto& p : my_2d_histos)    p.second->Write();
    for(const auto& p : my_3d_histos)    p.second->Write();
    for(const auto& p : my_2d_prof)      p.second->Write();
    for(const auto& p : my_1d_prof)      p.second->Write();
    for(const auto& p : my_efficiencies) p.second->Write();
    std::cout<<"Made it to the end of writing histos"<<std::endl;
}
